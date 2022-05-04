# Copyright (C) 2021 The uecho Authors. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

SHELL := bash

SOURCE_FILES= \
	manufacturers.c \
	objects_mra.c

.PHONY: all ${SOURCE_FILES} clean

all: ${SOURCE_FILES} clean

clean:
	@rm -f *.csv *.xlsx

############################################################
# Manufactuer 
############################################################

MANUFACTURE_XLSX=list_code_e.xlsx
MANUFACTURE_CSV=list_code_e.csv
MANUFACTURE_LIST_URL=https://echonet.jp/wp/wp-content/uploads/pdf/General/Echonet/ManufacturerCode_en/${MANUFACTURE_XLSX}

manufacturers.c : ${MANUFACTURE_CSV}
	@./manufacturers.pl ${MANUFACTURE_CSV} > $@

${MANUFACTURE_XLSX}:
	@wget -q -O ${MANUFACTURE_XLSX} ${MANUFACTURE_LIST_URL}

${MANUFACTURE_CSV}: ${MANUFACTURE_XLSX}
	@-ssconvert $< $@

############################################################
# Machine-readable database from Echonet consortium
############################################################

MRA_JSON_DIR=MRA_V1.0.0

objects_mra.c :
	@./objects_mra.pl ${MRA_JSON_DIR} > $@
