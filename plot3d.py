import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import griddata

# Leggi il file di testo
file_path = 'data3d.txt'

# Specifica il delimitatore usato nel file di testo, ad esempio '\t' per tabulazione o ' ' per spazio
data = pd.read_csv(file_path, delimiter='\t', dtype=str)

# Sostituisci le virgole con i punti decimali
data = data.apply(lambda x: x.str.replace(',', '.'))

# Converte i dati in formato numerico
data = data.astype(float)

# Controlla le prime righe del DataFrame per assicurarti che sia stato caricato correttamente
print(data.head())

# Estrai le colonne
x = data.iloc[:, 0].values
y = data.iloc[:, 1].values
z = data.iloc[:, 2].values

# Crea una griglia per l'interpolazione
grid_x, grid_y = np.meshgrid(np.linspace(x.min(), x.max(), 100), 
                             np.linspace(y.min(), y.max(), 100))

# Interpola i dati sulla griglia
grid_z = griddata((x, y), z, (grid_x, grid_y), method='cubic')

# Crea la figura e l'asse 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot della superficie interpolata
ax.plot_surface(grid_x, grid_y, grid_z, cmap='viridis', alpha=0.7)

# Aggiungi i punti di dati originali
ax.scatter(x, y, z, color='r', marker='o')

# Imposta le etichette degli assi
ax.set_xlabel('#elves')
ax.set_ylabel('#santa')
ax.set_zlabel('time average')

# Mostra il grafico
plt.show()
