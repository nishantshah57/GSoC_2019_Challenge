import json
import argparse
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def survival(x, y, grid):
    """
    Compute one iteration of Life for one cell.
    :param x: x coordinate of cell in the grid
    :type x: int
    :param y: y coordinate of cell in the grid
    :type y: int
    :param grid: the grid of cells
    :type grid: np.ndarray
    """
    num_neighbours = np.sum(grid[x - 1 : x + 2, y - 1 : y + 2]) - grid[x, y]
    # The rules of Life
    if grid[x, y] and not 2 <= num_neighbours <= 3:
        return 0
    elif num_neighbours == 3:
        return 1
    return grid[x, y]

def generation(grid):
    """
    Compute one iteration of Life for the grid.
    :param grid: initial grid of cells
    :type grid: np.ndarray
    :return: updated grid of cells
    :rtype: np.ndarray
    """
    new_grid = np.copy(grid)
    # Apply the survival function to every cell in the grid
    for i in range(grid.shape[0]):
        for j in range(grid.shape[1]):
            new_grid[i, j] = survival(i, j, grid)
    return new_grid

def animate_life(
    configurations,

    grid_size,
    conf,

    conf_position,
    # cmap="Purples",
    n_generations=50,
    interval=300,
    save=True,
    quality=200,
    # confs,
):
    """
    Animate the Game of Life.
    :param grid_size: dimensions of the grid
    :type grid_size: tuple (int, int)
    :param conf: initial starting array
    :type conf: list of lists, np.ndarray
    :param conf_position: coordinates where the top-left corner of the seed array should
                          be pinned
    :type conf_position: tuple (int, int)
    :param cmap: the matplotlib cmap that should be used
    :type cmap: str
    :param n_generations: number of grid iterations, defaults to 30
    :param n_generations: int, optional
    :param interval: time interval between updates (milliseconds), defaults to 300ms
    :param interval: int, optional
    :param save: whether the animation should be saved, defaults to False
    :param save: bool, optional
    """
    # Initialise the grid and seed
    grid = np.zeros(grid_size)
    x_start, y_start = conf_position[0], conf_position[1]
    conf_array = np.array(configurations[conf])
    x_end, y_end = x_start + conf_array.shape[0], y_start + conf_array.shape[1]
    grid[x_start:x_end, y_start:y_end] = conf_array

    # Animate
    fig = plt.figure(dpi=quality)
    plt.axis("off")
    ims = []
    for i in range(n_generations):
        ims.append((plt.imshow(grid, cmap='binary'),))
        grid = generation(grid)
    im_ani = animation.ArtistAnimation(
        fig, ims, interval=interval, repeat_delay=3000, blit=True
    )
    # Optional: save the animation, with a name based on the seed.
    if save:
        im_ani.save((str(conf) + ".gif"), writer="imagemagick")

def main():
    parser = argparse.ArgumentParser(description="Conway's Game of Life")
    parser.add_argument("--grid-size", type=str, default="100, 100", help=" Comma Separated dimensions (X, Y)")
    parser.add_argument("--conf", type=str, default="block_switch_engine",
                        help="See Readme List for number of available configurations")
    parser.add_argument("-n", type=int, default=50, help="Number of Iterations")
    parser.add_argument("-quality", type=int, default=1000, required=False, help="Image quality in DPI")
    parser.add_argument("-interval", type=int, default=300, help="interval (in milliseconds) between iterations")
    parser.add_argument("--conf-position", type=str, default="40, 40", help="Comma Separated coordinated of configuration")

    args = parser.parse_args()
    # configurations = {}
    with open("configurations.json", "r") as read_file:
        configurations = json.load(read_file)
        # print(confs['infinite'])

    animate_life(
        configurations,
        grid_size=(
            int(args.grid_size.split(",")[0]),
            int(args.grid_size.split(",")[1]),
        ),

        conf=args.conf,
        # cmap=args.cmap,
        conf_position=(
            int(args.conf_position.split(",")[0]),
            int(args.conf_position.split(",")[1]),
        ),
        n_generations=args.n,
        interval=args.interval,
        save=True,
        quality=args.quality,

    )

if __name__ == "__main__":
    main()