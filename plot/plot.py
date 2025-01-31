import matplotlib.pyplot as plt

x_vals = []
y_vals = []

with open('plot/pla88kpmx.txt', 'r') as f:
    for line in f:
        x, y = map(float, line.split())
        x_vals.append(x)
        y_vals.append(y)

plt.plot(x_vals, y_vals, color='blue')
plt.xlabel('Geração')
plt.ylabel('Distância média da população')
plt.title('pla85900: Evolução da distância média da população')
plt.savefig('plot/plot.png')