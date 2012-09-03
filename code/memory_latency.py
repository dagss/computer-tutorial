from __future__ import division

import numpy as np
from matplotlib import pyplot as plt
np.seterr(all='raise')

from memory_benchmarks_wrapper import benchmark_latency


ks = np.arange(8, 28)
ns = 2**ks
nrepeats = 2**(ks[-1] - ks)
times = np.zeros(len(ks), dtype=np.float64)

fig, ax = plt.subplots(1, 1)

# Linear access
print '== Linear'
for i, (n, nrepeat) in enumerate(zip(ns, nrepeats)):
    indices = np.arange(n, dtype=np.uint64) + 1
    trials = [benchmark_latency(indices, nrepeat) for j in range(5)]
    times[i] = time = np.min(trials)
    print 'size=%d, time=%e, repeats=%d' % (n, time, nrepeat)

ax.semilogx(ns, times / ns, '-o', basex=2, label='Linear access')

# Random access
print '== Random'
for i, (n, nrepeat) in enumerate(zip(ns, nrepeats)):
    indices = np.random.randint(n, size=n).astype(np.uint64)
    trials = [benchmark_latency(indices, nrepeat) for j in range(5)]
    times[i] = time = np.min(trials)
    print 'size=%d, time=%e, repeats=%d' % (n, time, nrepeat)
    
ax.semilogx(ns, times / ns, '-o', basex=2, label='Random access')

ax.set_ylim((0, ax.get_ylim()[1]))
ax.legend(loc='upper left')
plt.show()
