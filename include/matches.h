/*
 * PROJECT: GEMMapper
 * FILE: matches.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: Data structure to store alignment matches {sequence,position,strand,...}
 */

#ifndef MATCHES_H_
#define MATCHES_H_

#include "essentials.h"
#include "interval_set.h"
#include "text_collection.h"
#include "match_elements.h"
#include "swg_align.h"

/*
 * Checkers
 */
#define MATCHES_CHECK(matches) GEM_CHECK_NULL(matches)

/*
 * Interval Match
 */
typedef struct {
  /* Meta-info */
  bool emulated_rc_search; // Match resulting from a RC-emulated search (using the forward-strand)
  /* Index */
  uint64_t lo;             // SA Lo-Position
  uint64_t hi;             // SA Hi-Position
  /* Sequence */
  uint8_t* text;           // Pointer to the matching-text
  uint64_t text_length;    // Length of the matching-text
  strand_t strand;         // Mapping Strand
  /* Score */
  uint64_t distance;       // Distance of the alignment
  int32_t swg_score;       // SWG Distance (score) of the alignment
} match_interval_t;

/*
 * Position Match (Trace-Match)
 */
typedef struct {
  /* Match Text (Reference) */
  uint64_t trace_offset;   // Trace-offset in the text-collection
  uint8_t* text;           // Pointer to the matching-text
  uint64_t text_length;    // Length of the matching-text
  /* Match */
  char* sequence_name;     // Sequence name (After decoding.Eg Chr1)
  strand_t strand;         // Mapping Strand
  uint64_t text_position;  // Position of the match in the text. Local text (Eg wrt Chr1)
  bool emulated_rc_search; // Match resulting from a RC-emulated search (using the forward-strand)
  /* Score */
  uint64_t distance;       // Distance
  uint64_t edit_distance;  // Edit-Distance
  int32_t swg_score;       // SWG Distance/Score
  uint8_t mapq_score;      // MAPQ Score
  /* Alignment */
  match_alignment_t match_alignment; // Match Alignment (CIGAR + ...)
#ifdef GEM_DEBUG
  void* match_scaffold;    // Supporting Scaffolding
#endif
} match_trace_t;

/*
 * Matches-Metrics
 */
typedef struct {
  /*
   * Matches current metrics status (always being updated)
   */
  uint64_t min1_counter_value; // Minimum non-zero counter position (for the distance metric the counters use)
  uint64_t min2_counter_value; // Second minimum non-zero counter position (for the distance metric the counters use)
  uint64_t max_counter_value;  // Maximum non-zero counter position (for the distance metric the counters use)
  uint64_t min1_edit_distance; // Minimum edit distance among all found matches
  uint64_t min2_edit_distance; // Second minimum edit distance among all found matches
  int32_t max1_swg_score;      // Maximum smith-waterman-gotoh score among all found matches
  int32_t max2_swg_score;      // Second maximum smith-waterman-gotoh score among all found matches
  /*
   * Calculated metrics (from a given set of matches & sorting)
   */
  // First Map (Primary Match)
  uint64_t first_map_edit_distance;
  uint64_t first_map_event_distance;
  int32_t first_map_swg_score;
  double first_map_edit_distance_norm;
  double first_map_event_distance_norm;
  double first_map_swg_score_norm;
  // Sub-dominant Matches (best in each distance metric)
  uint64_t subdominant_edit_distance;
  uint64_t subdominant_event_distance;
  int32_t subdominant_swg_score;
  double subdominant_edit_distance_norm;
  double subdominant_event_distance_norm;
  double subdominant_swg_score_norm;
  // Search Scope
  uint64_t first_stratum_matches;
  uint64_t subdominant_stratum_matches;
  uint64_t mcs;
  uint64_t max_region_length;
  double max_region_length_norm;
} matches_metrics_t;

/*
 * Matches
 */
typedef struct {
  /* Search-matches state */
  uint64_t max_complete_stratum;
  /* Text Collection Buffer */
  text_collection_t* text_collection;       // Stores text-traces (candidates/matches/regions/...)
  /* Matches Counters */
  vector_t* counters;                       // Global counters
  uint64_t total_matches_count;             // Number of matches reflected in the counters (Interval+Position)
  /* Interval Matches */
  vector_t* interval_matches;               // Interval Matches (match_interval_t)
  /* Position Matches */
  vector_t* position_matches;               // Position Matches (match_trace_t)
  ihash_t* begin_pos_matches;               // Begin position (of the aligned match) in the text-space
  ihash_t* end_pos_matches;                 // End position (of the aligned match) in the text-space
  /* CIGAR */
  vector_t* cigar_vector;                   // CIGAR operations storage (cigar_element_t)
  /* Stats/Metrics */
  matches_metrics_t metrics;
} matches_t;

/*
 * Setup
 */
GEM_INLINE matches_t* matches_new();
GEM_INLINE void matches_configure(matches_t* const matches,text_collection_t* const text_collection);
GEM_INLINE void matches_clear(matches_t* const matches);
GEM_INLINE void matches_delete(matches_t* const matches);

/*
 * Accessors
 */
GEM_INLINE bool matches_is_mapped(const matches_t* const matches);

/*
 * Counters
 */
GEM_INLINE uint64_t matches_counters_compact(matches_t* const matches);
GEM_INLINE uint64_t matches_counters_get_count(matches_t* const matches,const uint64_t distance);
GEM_INLINE uint64_t matches_counters_get_total_count(matches_t* const matches);

/*
 * Index
 */
GEM_INLINE void matches_index_rebuild(matches_t* const matches,mm_stack_t* const mm_stack);
GEM_INLINE void matches_index_clear(matches_t* const matches);

/*
 * Matches
 */
GEM_INLINE match_trace_t* matches_get_match_traces(const matches_t* const matches);
GEM_INLINE uint64_t matches_get_num_match_traces(const matches_t* const matches);
GEM_INLINE void matches_get_clear_match_traces(const matches_t* const matches);

GEM_INLINE cigar_element_t* match_trace_get_cigar_buffer(const matches_t* const matches,const match_trace_t* const match_trace);
GEM_INLINE uint64_t match_trace_get_cigar_length(const match_trace_t* const match_trace);
GEM_INLINE uint64_t match_trace_get_event_distance(const match_trace_t* const match_trace);
GEM_INLINE int64_t match_trace_get_effective_length(
    matches_t* const matches,const uint64_t read_length,
    const uint64_t cigar_buffer_offset,const uint64_t cigar_length);

/*
 * Adding Matches
 */
GEM_INLINE bool matches_add_match_trace(
    matches_t* const matches,match_trace_t* const match_trace,
    const bool update_counters,mm_stack_t* const mm_stack);
GEM_INLINE void matches_add_interval_match(
    matches_t* const matches,const uint64_t lo,const uint64_t hi,
    const uint64_t length,const uint64_t distance,const bool emulated_rc_search);
GEM_INLINE void matches_add_interval_set(
    matches_t* const matches,interval_set_t* const interval_set,
    const uint64_t length,const bool emulated_rc_search);

GEM_INLINE void matches_hint_allocate_match_trace(matches_t* const matches,const uint64_t num_matches_trace_to_add);
GEM_INLINE void matches_hint_allocate_match_interval(matches_t* const matches,const uint64_t num_matches_interval_to_add);

/*
 * CIGAR Handling
 */
GEM_INLINE void matches_cigar_buffer_add_cigar_element(
    cigar_element_t** const cigar_buffer_sentinel,
    const cigar_t cigar_element_type,const uint64_t element_length);
GEM_INLINE void matches_cigar_buffer_add_mismatch(
    cigar_element_t** const cigar_buffer_sentinel,const uint8_t mismatch);

GEM_INLINE void matches_cigar_vector_append_insertion(
    vector_t* const cigar_vector,uint64_t* const current_cigar_length,
    const uint64_t indel_length,const cigar_attr_t attributes);
GEM_INLINE void matches_cigar_vector_append_deletion(
    vector_t* const cigar_vector,uint64_t* const current_cigar_length,
    const uint64_t indel_length,const cigar_attr_t attributes);
GEM_INLINE void matches_cigar_vector_append_match(
    vector_t* const cigar_vector,uint64_t* const current_cigar_length,
    const uint64_t match_length,const cigar_attr_t attributes);
GEM_INLINE void matches_cigar_vector_append_mismatch(
    vector_t* const cigar_vector,uint64_t* const current_cigar_length,
    const uint8_t mismatch,const cigar_attr_t attributes);
GEM_INLINE void matches_cigar_vector_append_cigar_element(
    vector_t* const cigar_vector,uint64_t* const cigar_length,cigar_element_t* const cigar_element);

GEM_INLINE void matches_cigar_reverse(
    matches_t* const matches,const uint64_t cigar_buffer_offset,const uint64_t cigar_length);
GEM_INLINE void matches_cigar_reverse_colorspace(
    matches_t* const matches,const uint64_t cigar_buffer_offset,const uint64_t cigar_length);

GEM_INLINE uint64_t matches_cigar_compute_event_distance(
    const matches_t* const matches,const uint64_t cigar_buffer_offset,const uint64_t cigar_length);
GEM_INLINE uint64_t matches_cigar_compute_edit_distance(
    const matches_t* const matches,const uint64_t cigar_buffer_offset,const uint64_t cigar_length);
GEM_INLINE uint64_t matches_cigar_compute_edit_distance__excluding_clipping(
    const matches_t* const matches,const uint64_t cigar_buffer_offset,const uint64_t cigar_length);

GEM_INLINE int64_t matches_cigar_element_effective_length(const cigar_element_t* const cigar_element);
GEM_INLINE int64_t matches_cigar_effective_length(
    vector_t* const cigar_vector,const uint64_t cigar_offset,const uint64_t cigar_length);

/*
 * Sorting Matches
 */
GEM_INLINE void matches_sort_by_distance(matches_t* const matches);
GEM_INLINE void matches_sort_by_swg_score(matches_t* const matches);
GEM_INLINE void matches_sort_by_mapq_score(matches_t* const matches);
GEM_INLINE void matches_sort_by_sequence_name__position(matches_t* const matches);

/*
 * Filters
 */
GEM_INLINE void matches_filter_by_swg_score(matches_t* const matches,const double swg_threshold,mm_stack_t* const mm_stack);
GEM_INLINE void matches_filter_by_mapq(matches_t* const matches,const uint8_t mapq_threshold,mm_stack_t* const mm_stack);

/*
 * Display
 */
GEM_INLINE void match_cigar_print(
    FILE* const stream,vector_t* const cigar_vector,
    const uint64_t cigar_buffer_offset,const uint64_t cigar_length);

/*
 * Error Messages
 */
#define GEM_ERROR_MATCHES_CIGAR_ZERO_LENGTH "Matches. CIGAR length cannot be zero"

#endif /* MATCHES_H_ */
