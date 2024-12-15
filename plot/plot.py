import matplotlib.pyplot as plt

x_vals = []
y_vals = []

with open('plot/data.txt', 'r') as f:
    for line in f:
        x, y = map(float, line.split())
        x_vals.append(x)
        y_vals.append(y)

plt.plot(x_vals, y_vals, color='red')
plt.xlabel('Tempo (s)')
plt.ylabel('Distância')
plt.title('Pair Swap: Distância x Tempo')
plt.savefig('plot/plot.png')