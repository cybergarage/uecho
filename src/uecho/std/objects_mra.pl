#!/usr/bin/perl
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
# Generates objects_mra.c, the standard object database, from the ECHONET
# Consortium MRA (Machine Readable Appendix) JSON files:
#
#   ./objects_mra.pl <MRA root directory> > objects_mra.c
#
# The output is a set of const tables, so the database lives in ROM (flash on
# MCUs) instead of being built on the heap at runtime. The generator fails
# instead of guessing when the MRA contains something it does not understand,
# so an MRA update cannot silently produce a wrong database.

use strict;
use warnings;
use utf8;
use JSON::PP;
use File::Basename;
use File::Find;
use Encode qw(encode);

if (@ARGV < 1) {
  print STDERR "usage: $0 <MRA root directory>\n";
  exit 1;
}
my $mra_root_dir = $ARGV[0];
$mra_root_dir =~ s/\/+$//;
my $mra_version = basename($mra_root_dir);

# Classes kept even when the database is excluded (UECHO_DATABASE_NONE):
# the library itself needs the super class and the node profile class.
my %essential_dirs = (
  "superClass"  => 1,
  "nodeProfile" => 1,
);
my @mra_sub_dirs = ("superClass", "nodeProfile", "devices");
my @required_classes = ("0000", "0EF0");

# MRA access rules -> uEchoPropertyAttr flags.
# required_c (conditionally required) and required_o (required when an option
# is supported) are supported but not mandatory for every device.
my %attr_flags = (
  get => {
    required      => "uEchoPropertyAttrReadRequired",
    required_c    => "uEchoPropertyAttrRead",
    required_o    => "uEchoPropertyAttrRead",
    optional      => "uEchoPropertyAttrRead",
    notApplicable => undef,
  },
  set => {
    required      => "uEchoPropertyAttrWriteRequired",
    required_c    => "uEchoPropertyAttrWrite",
    required_o    => "uEchoPropertyAttrWrite",
    optional      => "uEchoPropertyAttrWrite",
    notApplicable => undef,
  },
  inf => {
    required      => "uEchoPropertyAttrAnnoRequired",
    required_c    => "uEchoPropertyAttrAnno",
    required_o    => "uEchoPropertyAttrAnno",
    optional      => "uEchoPropertyAttrAnno",
    notApplicable => undef,
  },
);

sub read_json {
  my ($file) = @_;
  open(my $fh, "<:raw", $file) or die "Failed to open $file: $!\n";
  local $/;
  my $data = <$fh>;
  close($fh);
  my $json = eval { JSON::PP->new->utf8->decode($data) };
  die "Failed to parse $file: $@" unless defined $json;
  return $json;
}

# Converts a string to a C string literal. Non-ASCII characters are written as
# octal escapes of their UTF-8 bytes so the output is plain ASCII.
sub c_string {
  my ($str) = @_;
  my $bytes = encode("UTF-8", $str);
  my $out = "";
  foreach my $c (split(//, $bytes)) {
    my $o = ord($c);
    if ($c eq "\\" || $c eq "\"") {
      $out .= "\\" . $c;
    }
    elsif ($o < 0x20 || 0x7E < $o || $c eq "?") {
      # "?" is escaped to avoid trigraphs.
      $out .= sprintf("\\%03o", $o);
    }
    else {
      $out .= $c;
    }
  }
  return "\"" . $out . "\"";
}

sub hex_code {
  my ($value, $digits, $what) = @_;
  die "$what: missing code\n" unless defined $value;
  die "$what: invalid code '$value'\n" unless $value =~ /^0x([0-9A-Fa-f]{$digits})$/;
  return uc($1);
}

# An entry valid in the latest release wins over one that is not; otherwise
# the later entry wins.
sub is_latest {
  my ($entry) = @_;
  my $release = $entry->{validRelease};
  return 1 unless ref($release) eq "HASH" && defined $release->{to};
  return $release->{to} eq "latest" ? 1 : 0;
}

sub supersedes {
  my ($new, $old) = @_;
  return is_latest($new) || !is_latest($old);
}

sub attr_expr {
  my ($rules, $what) = @_;
  die "$what: missing accessRule\n" unless ref($rules) eq "HASH";
  my @flags;
  foreach my $op ("get", "set", "inf") {
    my $rule = $rules->{$op};
    die "$what: missing accessRule.$op\n" unless defined $rule;
    die "$what: unknown accessRule.$op '$rule'\n" unless exists $attr_flags{$op}{$rule};
    push(@flags, $attr_flags{$op}{$rule}) if defined $attr_flags{$op}{$rule};
  }
  return @flags ? join(" | ", @flags) : "uEchoPropertyAttrNone";
}

# Preserve the original data schemas and one shared definition dictionary.
# Validate local refs without flattening siblings/overrides or guessing semantics.
my $definitions_file = "$mra_root_dir/mraData/definitions/definitions.json";
my $definitions = -f $definitions_file ? read_json($definitions_file)->{definitions} : {};
die "Missing or invalid definitions dictionary\n" unless ref($definitions) eq "HASH";
sub validate_schema {
  my ($value, $seen) = @_;
  if (ref($value) eq "HASH") {
    if (exists $value->{'$ref'}) {
      my $ref = $value->{'$ref'};
      die "Unsupported schema reference $ref\n" unless $ref =~ m{^#/definitions/([^/]+)$};
      my $key = $1;
      $key =~ s/~1/\//g; $key =~ s/~0/~/g;
      die "Missing schema reference $ref\n" unless exists $definitions->{$key};
      die "Cyclic schema reference $ref\n" if $seen->{$key};
      validate_schema($definitions->{$key}, { %$seen, $key => 1 });
    }
    validate_schema($_, $seen) for values %$value;
  }
  elsif (ref($value) eq "ARRAY") { validate_schema($_, $seen) for @$value; }
}

validate_schema($_, {}) for values %$definitions;

# Read classes

my %classes;
foreach my $sub_dir (@mra_sub_dirs) {
  my $dir = "$mra_root_dir/mraData/$sub_dir";
  die "MRA directory not found: $dir\n" unless -d $dir;
  my @files;
  find(sub { push(@files, $File::Find::name) if -f $_ && /\.json$/ }, $dir);
  foreach my $file (sort @files) {
    my $json = read_json($file);
    my $code = hex_code($json->{eoj}, 4, $file);
    my $name = $json->{className}{en};
    die "$file: missing className.en\n" unless defined $name && length($name);

    my %props;
    my $order = 0;
    my $epcs = $json->{elProperties};
    die "$file: missing elProperties\n" unless ref($epcs) eq "ARRAY";
    foreach my $prop (@{$epcs}) {
      my $epc = hex_code($prop->{epc}, 2, $file);
      my $what = "$file: EPC 0x$epc";
      die "$what: EPC out of range\n" if hex($epc) < 0x80;
      my $prop_name = $prop->{propertyName}{en};
      die "$what: missing propertyName.en\n" unless defined $prop_name;
      die "$what: data schema is not an object\n" if exists $prop->{data} && ref($prop->{data}) ne "HASH";
      validate_schema($prop->{data}, {}) if exists $prop->{data};
      my $entry = {
        epc   => $epc,
        schema => exists $prop->{data} ? JSON::PP->new->canonical->encode($prop->{data}) : undef,
        name  => $prop_name,
        attr  => attr_expr($prop->{accessRule}, $what),
        latest => is_latest($prop),
        validRelease => $prop->{validRelease},
      };
      # A duplicated EPC takes the position of its last occurrence.
      $order++;
      if (exists $props{$epc}) {
        $entry = $props{$epc}{entry} unless supersedes($entry, $props{$epc}{entry});
      }
      $props{$epc} = { entry => $entry, order => $order };
    }

    my $class = {
      code      => $code,
      name      => $name,
      essential => $essential_dirs{$sub_dir} ? 1 : 0,
      props     => [map { $props{$_}{entry} } sort { $props{$a}{order} <=> $props{$b}{order} } keys %props],
      validRelease => $json->{validRelease},
    };
    if (exists $classes{$code}) {
      print STDERR "warning: duplicated class 0x$code in $file\n";
      next unless supersedes($class, $classes{$code});
    }
    $classes{$code} = $class;
  }
}

foreach my $code (@required_classes) {
  die "Required class 0x$code not found in $mra_root_dir\n" unless exists $classes{$code} && $classes{$code}{essential};
}

# Output

print <<"HEADER";
/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 * GENERATED FROM objects_mra.pl ($mra_version) DO NOT EDIT THIS FILE.
 * MRA-derived data: Copyright 2021-2024 ECHONET Consortium.
 * See MRA-COPYRIGHT.txt for the retained permission notice.
 *
 ******************************************************************/

#include <uecho/std/_standard.h>

#if defined(UECHO_DATABASE_VALUE_METADATA) && UECHO_DATABASE_VALUE_METADATA
#define UECHO_STD_SCHEMA(value) value
#define UECHO_STD_SCHEMA_FIELD(value) , value
#else
#define UECHO_STD_SCHEMA(value) NULL
#define UECHO_STD_SCHEMA_FIELD(value)
#endif

HEADER

print "const char* const uecho_std_value_definitions = UECHO_STD_SCHEMA(" . c_string(JSON::PP->new->canonical->encode($definitions)) . ");\n\n";

print "const char* const uecho_std_source_version = " . c_string($mra_version) . ";\n\n";

my @codes = sort keys %classes;

# Intern identical immutable property definitions once in generated source.
# Arrays retain their original order/layout, but repeated class fragments share
# one definition rather than duplicating long schema/attribute expressions.
my %property_ids;
my @property_definitions;
foreach my $code (@codes) {
  foreach my $prop (@{$classes{$code}{props}}) {
    my $signature = JSON::PP->new->canonical->encode([@$prop{qw(epc attr name schema)}]);
    if (!exists $property_ids{$signature}) {
      $property_ids{$signature} = scalar @property_definitions;
      push @property_definitions, { prop => $prop, essential => 0 };
    }
    my $id = $property_ids{$signature};
    $property_definitions[$id]{essential} ||= $classes{$code}{essential};
    $prop->{definition_id} = $id;
  }
}
foreach my $id (0 .. $#property_definitions) {
  my $entry = $property_definitions[$id];
  my $prop = $entry->{prop};
  print "#if !defined(UECHO_DATABASE_NONE)\n" unless $entry->{essential};
  printf("#define UECHO_STD_PROPERTY_%04d { 0x%s, %s, %s UECHO_STD_SCHEMA_FIELD(%s) }\n", $id, $prop->{epc}, $prop->{attr}, c_string($prop->{name}), defined $prop->{schema} ? c_string($prop->{schema}) : "NULL");
  print "#endif\n" unless $entry->{essential};
}
print "\n";

foreach my $code (@codes) {
  my $class = $classes{$code};
  next unless @{$class->{props}};
  print "#if !defined(UECHO_DATABASE_NONE)\n" unless $class->{essential};
  printf("// %s (0x%s)\n", $class->{name}, $code);
  printf("static const uEchoStdProperty uecho_std_props_%s[] = {\n", $code);
  foreach my $prop (@{$class->{props}}) {
    printf("  UECHO_STD_PROPERTY_%04d,\n", $prop->{definition_id});
  }
  print "};\n";
  print "#endif\n" unless $class->{essential};
  print "\n";
}

print "const uEchoStdObject uecho_std_objects[] = {\n";
my $guarded = 0;
foreach my $code (@codes) {
  my $class = $classes{$code};
  if (!$class->{essential} && !$guarded) {
    print "#if !defined(UECHO_DATABASE_NONE)\n";
    $guarded = 1;
  }
  elsif ($class->{essential} && $guarded) {
    print "#endif\n";
    $guarded = 0;
  }
  my ($grp, $cls) = (substr($code, 0, 2), substr($code, 2, 2));
  if (@{$class->{props}}) {
    printf("  { 0x%s, 0x%s, %s, uecho_std_props_%s, sizeof(uecho_std_props_%s) / sizeof(uecho_std_props_%s[0]) },\n", $grp, $cls, c_string($class->{name}), $code, $code, $code);
  }
  else {
    printf("  { 0x%s, 0x%s, %s, NULL, 0 },\n", $grp, $cls, c_string($class->{name}));
  }
}
print "#endif\n" if $guarded;
print "};\n\n";
print "const size_t uecho_std_objectcount = sizeof(uecho_std_objects) / sizeof(uecho_std_objects[0]);\n";
