from __future__ import division

import numpy as np
from matplotlib import pyplot as plt
np.seterr(all='raise')

from memory_benchmarks_wrapper import benchmark_bandwidth

max_k = 25
sizes = np.array([2**k for k in range(3, max_k)])
nrepeats = np.array([2**(max_k + 1 - k) for k in range(3, max_k)])
times = np.zeros(len(sizes), dtype=np.float64)

for i, (size, nrepeat) in enumerate(zip(sizes, nrepeats)):
    a = np.zeros(size)
    trials = [benchmark_bandwidth(a, repeats=nrepeat) for j in range(5)]
    times[i] = time = np.min(trials)
    print 'size=%d, time=%e, repeats=%d' % (size, time, nrepeat)

rates = (sizes * 8) / times

fig, ax = plt.subplots(1, 1)
ax.semilogx(sizes, rates, '-o', basex=2)
ax.set_ylim((0, 5e9))

