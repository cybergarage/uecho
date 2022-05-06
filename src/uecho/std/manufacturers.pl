#!/usr/bin/perl
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.

if (@ARGV < 1){
  exit 1;
}
my $manlist_filename = $ARGV[0];

print<<HEADER;
/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 * GENERATED FROM manufacturers.pl DO NOT EDIT THIS FILE.
 *
 ******************************************************************/

#include <uecho/std/_database.h>

bool uecho_database_addstandardmanufacture(uEchoDatabase* db, int code, const char *name) {
  uEchoManufacture *man;
  man = uecho_manufacture_new();
  uecho_manufacture_setcode(man, code);
  uecho_manufacture_setname(man, name);
  return uecho_database_addmanufacture(db, man); 
}

void uecho_database_addstandardmanufactures(uEchoDatabase* db) {
HEADER

open(MANLIST, $manlist_filename) or die "$!";
while(<MANLIST>){
  chomp($_);
  $_ =~ s/(['"].*?['"])/(my $s = $1) =~ tr|,|=|; $s/eg;
  my @row = split(/(?!"),/, $_, -1);;
  my $code = $row[0];
  if (length($code ) != 6) {
    next;
  }
  my $name = $row[1];
  $name =~ s/=/,/g;
  $name =~ s/　/ /g; # converts zenkaku spaces to spaces
  if ($name !~ /^\"/) {
    $name = "\""  . $name
  }
  if ($name !~ /\"$/) {
    $name = $name . "\"" 
  }
  printf("  uecho_database_addstandardmanufacture(db, 0x%s, %s);\n", $code, $name);
}
close(MANLIST);
print<<FOTTER;
}
FOTTER