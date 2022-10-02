import matplotlib.pyplot as plt
import numpy as np
import csv



def read_results(sample_name: str) -> list[dict[str,str]]: 
    num_rows = []
    with open(sample_name, newline='') as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            num_rows.append({key:row[key] for key in row})
        return num_rows

def extract_column(table: list[dict[str,str]], key: str, dtype) -> np.ndarray:
    col = []
    for row in table:
        col.append(row[key])
    return np.array(col, dtype= dtype)

def filter_table(table: list[dict[str,str]], filter_funs: list[callable]) -> list[dict[str,str]]:
    return [row for row in table for fun in filter_funs if fun(row)]

def filter_by_block_len(table: list[dict[str,str]], block_len: int) -> list[dict[str,str]]:
    return filter_table(table, [lambda row: int(row["len_bloque"]) == block_len])

def plot_performances(str_lens: np.ndarray, 
             alg_times: np.ndarray, 
             alg_labels: list[str], 
             block_len: int=0,
             log2: bool=False, 
             savefig_as: str=""):
    plt.rcParams['figure.figsize'] = [10,7]
    fig = plt.figure() 
    #plt.gcf().subplots_adjust(bottom=0.15, top=0.8)
    #ax = fig.add_axes([0.1, 0.1, 0.8, 0.8])

    xlabel = "Tamaño de los strings"
    ylabel = "Tiempo promedio [μs] algoritmos"

    if block_len < 1:
        title  = "Medición Performance de Distancia de Strings"
    else:
        title  = "Medición Performance de Distancia de Strings (B = {})".format(block_len)

    if log2:
        plt.loglog(str_lens, alg_times, "-")
    else:
        plt.plot(str_lens, alg_times, "-")
    plt.legend(alg_labels)

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)

    plt.grid(b=True)

    if savefig_as:
        plt.savefig(savefig_as, bbox_inches="tight")

    plt.show()

    


if __name__ == "__main__":
    result_filename = "time_results.csv"

    #block_size = 32
    #log2 = False
    #plot_filename = "plot_results.png"
    print()

    try:
        block_size = int(input("Ingrese tamaño de bloque usado en las mediciones: "))
    except ValueError:
        block_size = 0
    log2 = input("Usar escala logarítmica? (Y/N:default): ")
    log2 = log2.lower() == "y"
    plot_filename = input("Guardar imagen del plot? (Y:default/N): ")
    plot_filename = "" if plot_filename.lower() == "n" else "plot_results.png"

    results = read_results(result_filename)

    if block_size > 0:
        results = filter_by_block_len(results, block_size)
        
    results.sort(key= lambda row: int(row["len_string"]))

    string_sizes = extract_column(results, "len_string", int)
    algorithm_label_dict = {
        "t_dinamico[μs]": "Algoritmo Sin Optimizar",
        "t_cache[μs]": "Algoritmo Cache",
        #"t_cache2[μs]": "Algortimo Cache 2",
        "t_particion[μs]": "Algoritmo Submatrices"
    }
    algorithm_labels = [algorithm_label_dict[key] for key in algorithm_label_dict]

    algorithm_times = np.zeros((len(string_sizes), len(algorithm_labels)))
    i = 0
    for key in algorithm_label_dict:
        algorithm_times[:,i] = extract_column(results, key, float)
        i += 1

    plot_performances(string_sizes, algorithm_times, algorithm_labels, block_size, log2, plot_filename)
