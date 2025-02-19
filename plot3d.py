
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import griddata

FILE_PATH = 'data3d.txt'

def main():
    data = pd.read_csv(FILE_PATH, delimiter='\t', dtype=str)

    # Replace commas with dots in decimal numbers
    data = data.apply(lambda x : x.str.replace(',', '.'))

    # Convert data
    data = data.astype(float)

    # Print first record for read checking
    print(data.head())

    # Retrieve columns
    x = data.iloc[:, 0].values
    y = data.iloc[:, 1].values
    z = data.iloc[:, 2].values

    # Create grid to interpolate data
    grid_x, grid_y = np.meshgrid(np.linspace(x.min(), x.max(), 100), np.linspace(y.min(), y.max(), 100))

    # Interpolate data on grid
    grid_z = griddata((x, y), z, (grid_x, grid_y), method='cubic')

    # Plot
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(grid_x, grid_y, grid_z, cmap='viridis', alpha=0.7) # surface
    ax.scatter(x, y, z, color='r', marker='o') # original points

    # Axe labels
    ax.set_xlabel('#elves')
    ax.set_ylabel('#santa')
    ax.set_zlabel('time average')

    # LaTex style
    #plt.rcParams['font.sans-serif'] = 'Latin Modern Math'
    #plt.rcParams['font.family'] = 'sans-serif'

    # Show graph
    plt.show()


if __name__ == '__main__':
    main()
