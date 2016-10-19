/*
 *  GEM-Mapper v3 (GEM3)
 *  Copyright (c) 2011-2017 by Santiago Marco-Sola  <santiagomsola@gmail.com>
 *
 *  This file is part of GEM-Mapper v3 (GEM3).
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * PROJECT: GEM-Mapper v3 (GEM3)
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef ALIGNMENT_FILTERS_H_
#define ALIGNMENT_FILTERS_H_

#include "utils/essentials.h"
#include "align/align_bpm_pattern.h"
#include "align/align_kmer_filter.h"

typedef struct {
  /* Tile attributes */
  uint64_t tile_offset;
  uint64_t tile_length;
  uint64_t max_error;
  /* Filters */
  bpm_pattern_t* bpm_pattern_tile;
  kmer_counting_t* kmer_filter_tile;
} alignment_filters_tile_t;
typedef struct {
  /* Tile dimensions */
  uint64_t num_tiles;
  uint64_t tile_length;
  /* Global filters */
  bpm_pattern_t* bpm_pattern; // BPM filter
  /* Tiled filters */
  alignment_filters_tile_t* tiles;
  /* MM */
  mm_stack_t* mm_stack;
} alignment_filters_t;

/*
 * Compile alignment filters & tiles
 */
void alignment_filters_compile(
    alignment_filters_t* const alignment_filters,
    uint8_t* const key,
    const uint64_t key_length,
    const uint64_t max_error,
    mm_stack_t* const mm_stack);

#endif /* ALIGNMENT_FILTERS_H_ */
