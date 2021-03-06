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
 *   Archive-Search Single-End main module
 */

#ifndef ARCHIVE_SEARCH_SE_H_
#define ARCHIVE_SEARCH_SE_H_

#include "utils/essentials.h"
#include "archive/search/archive_search.h"
#include "matches/classify/matches_predictors.h"

/*
 * Archive Search SE Continue
 */
void archive_search_se_continue(
    archive_search_t* const archive_search,
    matches_t* const matches);

/*
 * Single-End Indexed Search (SE Online Approximate String Search)
 */
void archive_search_se(
    archive_search_t* const archive_search,
    matches_t* const matches);

/*
 * Display
 */
void archive_search_se_print(
    FILE* const stream,
    archive_search_t* const archive_search,
    matches_t* const matches);

#endif /* ARCHIVE_SEARCH_SE_H_ */
