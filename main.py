import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv('spline_data.csv')

num_nodes = 10

def f(x):
    return 8.0 / (np.cosh(2.0 * x) ** 2)

plt.figure(figsize=(10, 6))
plt.plot(data['x'], data['original_f(x)'], label='Функція f(x) = 8/ch^2(2x)', linewidth=2, color='blue', alpha=0.5)
plt.plot(data['x'], data['spline_S(x)'], label='Кубічний сплайн', linestyle='--', color='red')

nodes_x = np.linspace(-4.0, 4.0, num_nodes)
nodes_y = f(nodes_x)
plt.scatter(nodes_x, nodes_y, label='Точки інтерполяції', color='black', s=30, zorder=3)

plt.title('Інтерполяція функції кубічним сплайном')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.savefig('spline_plot.png')
plt.show()