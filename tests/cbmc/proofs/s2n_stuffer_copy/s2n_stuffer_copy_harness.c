/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <assert.h>
#include <cbmc_proof/cbmc_utils.h>
#include <cbmc_proof/make_common_datastructures.h>

#include <error/s2n_errno.h>

#include "api/s2n.h"
#include "stuffer/s2n_stuffer.h"

void s2n_stuffer_copy_harness()
{
    struct s2n_stuffer *from = cbmc_allocate_s2n_stuffer();
    __CPROVER_assume(s2n_result_is_ok(s2n_stuffer_validate(from)));
    struct s2n_stuffer            old_stuffer = *from;
    struct store_byte_from_buffer old_byte;
    save_byte_from_blob(&from->blob, &old_byte);
    struct s2n_stuffer *to = cbmc_allocate_s2n_stuffer();
    __CPROVER_assume(s2n_result_is_ok(s2n_stuffer_validate(to)));
    uint32_t length;

    nondet_s2n_mem_init();

    s2n_stuffer_copy(from, to, length);

    /* These assertions should always hold, regardless of whether the test succeeded */
    assert(from->blob.data == old_stuffer.blob.data);
    assert(from->blob.size == old_stuffer.blob.size);
    assert(from->write_cursor == old_stuffer.write_cursor);
    assert(from->high_water_mark == old_stuffer.high_water_mark);
    assert(from->alloced == old_stuffer.alloced);
    assert(from->growable == old_stuffer.growable);
    assert(from->tainted == old_stuffer.tainted);
    assert_byte_from_blob_matches(&from->blob, &old_byte);
    assert(s2n_result_is_ok(s2n_stuffer_validate(from)));
}
