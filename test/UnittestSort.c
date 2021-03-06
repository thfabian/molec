/*                   _
 *   _ __ ___   ___ | | ___  ___
 *  | '_ ` _ \ / _ \| |/ _ \/ __|
 *  | | | | | | (_) | |  __/ (__
 *  |_| |_| |_|\___/|_|\___|\___| - Molecular Dynamics Framework
 *
 *  Copyright (C) 2016  Carlo Del Don  (deldonc@student.ethz.ch)
 *                      Michel Breyer  (mbreyer@student.ethz.ch)
 *                      Florian Frei   (flofrei@student.ethz.ch)
 *                      Fabian Thuring (thfabian@student.ethz.ch)
 *
 *  This file is distributed under the MIT Open Source License.
 *  See LICENSE.txt for details.
 */

#include "Unittest.h"
#include <molec/Sort.h>
#include <molec/Periodic.h>

/**
 * Test comparison implementation
 */
TEST_CASE(molec_UnittestCompare)
{
    molec_Sort_Pair_t pair1, pair2;

    // pair1 goes before pair2
    pair1.key1 = 1.8;
    pair1.key2 = 0.2;
    pair1.value = 0;

    pair2.key1 = 1.9;
    pair2.key2 = 2.3;
    pair2.value = 1;

    CHECK(molec_compare(&pair1, &pair2) == -1);

    // pair1 goes after pair2
    pair1.key1 = 2.0;
    CHECK(molec_compare(&pair1, &pair2) == 1);
}

/**
 * Test sorting implementation
 */
TEST_CASE(molec_UnittestSort)
{
    // Initialize simulation
    molec_Simulation_SOA_t* sim = molec_setup_simulation_SOA();

    const int N = molec_parameter->N;
    const float L_x = molec_parameter->L_x;
    const float L_y = molec_parameter->L_y;
    const float L_z = molec_parameter->L_z;

    // Randomly move the atoms
    for(int i = 0; i < N; ++i)
    {
        sim->x[i] += (rand() / (float) RAND_MAX) * L_x;
        sim->y[i] += (rand() / (float) RAND_MAX) * L_y;
        sim->z[i] += (rand() / (float) RAND_MAX) * L_z;
    }

    // Apply periodic boundary conditions
    molec_periodic_refrence(sim->x, N, L_x);

    // Sort the particles according to the x coordinate
    molec_sort_qsort(sim);

    // Check if all the particles are sorted in x direction
    for(int i = 0; i < N - 1; ++i)
        CHECK_LE_FLOAT(sim->x[i], sim->x[i + 1]);

    molec_teardown_simulation_SOA(sim);
}

