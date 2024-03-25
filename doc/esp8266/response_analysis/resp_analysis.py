import matplotlib.pyplot as plt
import sys

f = open(sys.argv[1],'r')

BOI0 = int(sys.argv[2]) # Byte Of Interes 0
BOI1 = int(sys.argv[3])

if (BOI1 > 2): BOI1 = 2

xor_results = []
headers = []

MAX_HISTOGRAM = 256
_histogram = [0]* MAX_HISTOGRAM

for line in f:
  headers.append(line[:-1])
  corrected_l = line[:-1] + '\r\n';

  # u=10, max_conflicts=0, BOI 0-2
  xor_tmp = (ord(corrected_l[BOI0])) ^ (ord(corrected_l[BOI1]) << 1) ;
  
  xor_tmp = xor_tmp & 255
  _histogram[xor_tmp] += 1
  xor_results.append(xor_tmp)
  print "{} {}".format(line, xor_tmp)
f.close()

unique = 0
peak = 0
for i in _histogram:
  unique += 1*(i==1)
  if (i > peak) : peak = i;
peak = peak - 1;
print("uniques {}, max_conflicts {}, with:".format(unique, peak), BOI0, BOI1)

fig, ax = plt.subplots()
'''
barc = ax.bar(headers,xored)
#ax.invert_yaxis()
plt.xticks(rotation=90)
plt.subplots_adjust(bottom=0.3)
'''
labels = range(0, MAX_HISTOGRAM)
barc = ax.bar(labels, _histogram)
plt.show()

