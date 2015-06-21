/*
 * PROJECT: GEMMapper
 * FILE: filtering_candidates.h
 * DATE: 06/06/2013
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef FILTERING_CANDIDATES_H_
#define FILTERING_CANDIDATES_H_

#include "essentials.h"

#include "archive.h"
#include "locator.h"
#include "fm_index.h"
#include "pattern.h"
#include "bpm_align_gpu.h"
#include "approximate_search_parameters.h"
#include "region_profile.h"
#include "interval_set.h"
#include "filtering_region.h"
#include "matches.h"
#include "paired_matches.h"

/*
 * Filtering Candidates Vector
 */
typedef struct {
  /* Region Buffer */
  vector_t* regions_buffer;                   // Regions Buffer (region_t)
  /* Candidates */
  vector_t* filtering_positions;              // Candidate positions (filtering_position_t)
  vector_t* filtering_regions;                // Candidate regions (filtering_region_t)
  vector_t* discarded_regions;                // Discarded regions (filtering_region_t)
  vector_t* verified_regions;                 // Verified regions (verified_region_t)
  /* Auxiliary */
  vector_t* filtering_regions_locator;
} filtering_candidates_t;

/*
 * Setup
 */
GEM_INLINE void filtering_candidates_init(filtering_candidates_t* const filtering_candidates);
GEM_INLINE void filtering_candidates_clear(filtering_candidates_t* const filtering_candidates);
GEM_INLINE void filtering_candidates_destroy(filtering_candidates_t* const filtering_candidates);

/*
 * Accessors
 */
GEM_INLINE uint64_t filtering_candidates_get_num_candidate_regions(const filtering_candidates_t* const filtering_candidates);
GEM_INLINE uint64_t filtering_candidates_count_candidate_regions(
    filtering_candidates_t* const filtering_candidates_end,const filtering_region_status_t filtering_region_status);

GEM_INLINE void filtering_candidates_set_all_regions_pending(filtering_candidates_t* const filtering_candidates);
GEM_INLINE void filtering_candidates_set_all_regions_unverified(filtering_candidates_t* const filtering_candidates);

/*
 * Adding candidate positions
 */
GEM_INLINE void filtering_candidates_add_interval(
    filtering_candidates_t* const filtering_candidates,
    const uint64_t interval_lo,const uint64_t interval_hi,const uint64_t region_start_pos,
    const uint64_t region_end_pos,const uint64_t region_errors,mm_stack_t* const mm_stack);
GEM_INLINE void filtering_candidates_add_interval_set(
    filtering_candidates_t* const filtering_candidates,interval_set_t* const interval_set,
    const uint64_t region_start_pos,const uint64_t region_end_pos,mm_stack_t* const mm_stack);
GEM_INLINE void filtering_candidates_add_interval_set_thresholded(
    filtering_candidates_t* const filtering_candidates,interval_set_t* const interval_set,
    const uint64_t region_start_pos,const uint64_t region_end_pos,const uint64_t max_error,
    mm_stack_t* const mm_stack);

/*
 * Processing & Verification
 */
GEM_INLINE uint64_t filtering_candidates_process_candidates(
    filtering_candidates_t* const filtering_candidates,
    archive_t* const archive,const pattern_t* const pattern,
    const as_parameters_t* const as_parameters,
    const bool compose_region_chaining,mm_stack_t* const mm_stack);
GEM_INLINE uint64_t filtering_candidates_verify_candidates(
    filtering_candidates_t* const filtering_candidates,archive_t* const archive,
    text_collection_t* const text_collection,const pattern_t* const pattern,
    const as_parameters_t* const as_parameters,
    matches_t* const matches,mm_stack_t* const mm_stack);
GEM_INLINE uint64_t filtering_candidates_align_candidates(
    filtering_candidates_t* const filtering_candidates,archive_text_t* const archive_text,
    text_collection_t* const text_collection,pattern_t* const pattern,
    const bool emulated_rc_search,const as_parameters_t* const as_parameters,
    const bool approximated_distance,matches_t* const matches,mm_stack_t* const mm_stack);

/*
 * Search for local-alignments
 */
GEM_INLINE uint64_t filtering_candidates_local_alignment(
    filtering_candidates_t* const filtering_candidates,archive_t* const archive,
    text_collection_t* const text_collection,pattern_t* const pattern,
    const bool emulated_rc_search,const as_parameters_t* const as_parameters,
    matches_t* const matches,mm_stack_t* const mm_stack);

/*
 * BPM-Buffer API (Verification)
 */
GEM_INLINE uint64_t filtering_candidates_bpm_buffer_add(
    filtering_candidates_t* const filtering_candidates,
    pattern_t* const pattern,bpm_gpu_buffer_t* const bpm_gpu_buffer);
GEM_INLINE uint64_t filtering_candidates_bpm_buffer_retrieve(
    filtering_candidates_t* const filtering_candidates,archive_text_t* const archive_text,
    text_collection_t* const text_collection,pattern_t* const pattern,
    bpm_gpu_buffer_t* const bpm_gpu_buffer,const uint64_t candidate_offset_begin,
    const uint64_t candidate_offset_end,mm_stack_t* const mm_stack);

/*
 * Pair Extension
 */
GEM_INLINE uint64_t filtering_candidates_extend_match(
    filtering_candidates_t* const filtering_candidates,
    archive_t* const archive,text_collection_t* const text_collection,
    const match_trace_t* const extended_match,pattern_t* const candidate_pattern,
    const bool candidate_emulated_rc_search,const bool search_onward,
    const as_parameters_t* const candidate_actual_parameters,
    mapper_stats_t* const mapper_stats,paired_matches_t* const paired_matches,
    const sequence_end_t candidate_end,mm_stack_t* const mm_stack);
GEM_INLINE void filtering_candidates_process_extension_candidates(
    filtering_candidates_t* const extended_filtering_candidates,
    filtering_candidates_t* const candidate_filtering_candidates,
    archive_t* const archive,text_collection_t* const text_collection,
    const pattern_t* const extended_pattern,const pattern_t* const candidate_pattern,
    const search_parameters_t* const search_parameters,mapper_stats_t* const mapper_stats,
    paired_matches_t* const paired_matches,mm_stack_t* const mm_stack);

/*
 * Paired Filtering
 */
GEM_INLINE void filtering_candidates_paired_regions_filtering(
    filtering_candidates_t* const filtering_candidates_end1,
    filtering_candidates_t* const filtering_candidates_end2,
    archive_text_t* const archive_text,const search_parameters_t* const search_parameters,
    mapper_stats_t* const mapper_stats,paired_matches_t* const paired_matches);

/*
 * Display
 */
GEM_INLINE void filtering_candidates_print_matching_regions(
    FILE* const stream,filtering_candidates_t* const filtering_candidates);

#endif /* FILTERING_CANDIDATES_H_ */
