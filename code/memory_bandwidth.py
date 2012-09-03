from __future__ import division

import numpy as np
from matplotlib import pyplot as plt
np.seterr(all='raise')

from memory_benchmarks_wrapper import benchmark_bandwidth, get_num_threads


ks = np.arange(8, 28)
sizes = 2**ks
nrepeats = 2**(ks[-1] + 3 - ks)
times = np.zeros(len(ks), dtype=np.float64)

fig, ax = plt.subplots(1, 1)

for nthreads in [1, 2, 3, 4]:
    print '== nthreads=%d' % nthreads
    for i, (size, nrepeat) in enumerate(zip(sizes, nrepeats)):
        a = np.ones(size)
        trials = [benchmark_bandwidth(a, nthreads, nrepeat) for j in range(5)]
        times[i] = time = np.min(trials)
        print 'size=%d, time=%e, repeats=%d' % (size, time, nrepeat)

    rates = nthreads * sizes * 8 / times
    ax.semilogx(sizes, rates, '-o', basex=2, label='%d threads' % nthreads)
    
ax.set_ylim((0, ax.get_ylim()[1]))
ax.legend()
plt.show()
