# Path: graphs/plot.py
import matplotlib.pyplot as plt
import math
def plot(x, y, title, xlabel, ylabel, filename):
    plt.plot(x, y,linestyle='dashed', marker='o')
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.savefig(filename)
    plt.close()



def main():
    x = [1, 2, 3, 4, 5, 6]
    y = [2 * j+1 for j in x]
    plot(x,y  , "Liveness for modified bft", "faulty nodes", "total nodes required", "simple.png")
    a = [3, 4, 5, 6, 7, 8, 9, 10]
    b = [math.floor((j-1)/2) for j in a]
    plot(a,b  , "Safety for modified pbft", "Total nodes", "faulty nodes", "simple1.png")
    c = [ 4, 5, 6, 7, 8, 9, 10, 11, 12]
    d = [math.floor((j-1)/3) for j in c]
    plot(c,d  , "Safety for pbft", "Total nodes","faulty nodes", "simple2.png")

if __name__ == "__main__":
    main()