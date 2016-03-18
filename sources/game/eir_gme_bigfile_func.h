#pragma once

#include "eir_gme_bigfile.h"
#include "../kernel/eir_ker_env.h"

bool eir_gme_parse_bigfile(
	const char * bigfile_path,
	eir_gme_bigfile_t * bigfile
	);

bool eir_gme_generate_all_entities(
	const eir_gme_bigfile_t * bigfile,
	eir_ker_env_t * env
	);

void eir_gme_release_bigfile(
	eir_gme_bigfile_t * bigfile
	);
