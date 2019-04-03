"""
Utility functions
"""

import numpy as np
from numpy.random import random

def power_law_sequence(N, min_val, max_val, exponent):
    """power_law_sequence returns a sequence of value distributed
    according to a power law distribution.

    :param N: Size of the sequence
    :param min_val: Minimal value for the sequence elements
    :param max_val: Maximal value for the sequence elements
    :param exponent: Exponent of the power law distribution
    """
    value_vector = np.arange(min_val,max_val+1)
    value_distribution = value_vector**(-float(exponent))
    value_distribution /= np.sum(value_distribution)
    cumulative_value_distribution = np.cumsum(value_distribution)
    sequence = []
    for i in range(N):
        u = random()
        m = value_vector[np.searchsorted(cumulative_value_distribution, u)]
        sequence.append(m)
    return sequence

def clique_size_power_law_sequence(nmin, nmax, clique_exponent, membership_sequence):
    """clique_size_power_law_sequence returns a sequence of clique size
    distributed according to a power law distribution, constrained by a
    membership sequence.

    :param nmin: Minimal clique size.
    :param nmax: Maximal clique size.
    :param clique_exponent: Clique size power law distribution exponent.
    :param membership_sequence: membership sequence for nodes.
    """
    membership_stub = np.sum(membership_sequence)
    #clique size distribution cdf
    clique_size_vector = np.arange(nmin,nmax+1)
    clique_size_distribution = clique_size_vector**(-float(clique_exponent))
    clique_size_distribution /= np.sum(clique_size_distribution)
    cumulative_clique_size_distribution = np.cumsum(clique_size_distribution)
    #generate clique until the sum is coherent with the membership
    clique_size_sequence = []
    clique_stub = 0
    while (clique_stub != membership_stub):
        if clique_stub < membership_stub:
            u = random()
            n = clique_size_vector[np.searchsorted(cumulative_clique_size_distribution, u)]
            clique_size_sequence.append(n)
            clique_stub += n
        elif clique_stub > membership_stub:
            index = int(np.floor(random()*len(clique_size_sequence)))
            clique_size_sequence[index], clique_size_sequence[-1] = clique_size_sequence[-1], clique_size_sequence[index]
            clique_stub -= clique_size_sequence.pop()
    return clique_size_sequence
