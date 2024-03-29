#include <bits/stdc++.h>
using namespace std;

//#define USE_STACK
#define USE_VECTOR
//#define USE_NEW

//#define COMPUTE_CACHE2

// El algoritmo tambien funciona en strings de tamaño distintos
//  O(n*m) ???
int EditDistDynamic(string a, string b) // Algoritmo-1 Distancia de Edicion Programacion dinamica
{
    int filas = a.length();
    int columnas = b.length();

    // Aquí creamos una matriz de tamaño n x m
    #ifdef USE_STACK
    int res[filas + 1][columnas + 1] = {{0}}; // grilla de n+1, m+1
    #endif

    #ifdef USE_VECTOR
    vector< vector<int> > res( filas+1 , vector<int>(columnas+1) );
    #endif 

    #ifdef USE_NEW
    // inicializar arreglo de filas en heap con new (versión cheta de malloc)
    int** res { new int*[filas+1]{ } };
    for (int i{ 0 }; i < filas+1; i++)
    {
        // cada fila también debe ser inicializada con new
        res[i]  = new int[columnas+1]{ };
    }
    #endif
    
    for (int i = 0; i <= filas; i++)          // llenar filas
        res[i][0] = i;
    for (int j = 0; j <= columnas; j++) // llenar columnas
        res[0][j] = j;
    for (int i = 1; i <= filas; i++) // llenar matriz prog dinamica
    {
        for (int j = 1; j <= columnas; j++)
        {
            if (a[i - 1] != b[j - 1])
            {
                res[i][j] = min(1 + res[i - 1][j - 1], min(1 + res[i - 1][j], 1 + res[i][j - 1])); 
                // la casilla es determinado por el min entre las casillas aanteriores + 1
            }
            else
            {
                res[i][j] = res[i - 1][j - 1]; // si son iaguales los caracteres no hay costo por sobreescribir
            }
        }
    }
    int finalResult{ res[filas][columnas] };

    #ifdef USE_NEW
    // delete allocated memory
    for (int i{ 0 }; i < filas+1; i++)
    {
        delete res[i];
    }
    delete res;
    #endif

    return finalResult;
}

//no funciona en strigns de largos distinto:c TODO: revisar pq'
//O(2*m)?? la verdad ni siquiera lo pense pero se ve como algo por el estilo
int EditDistCache(string a, string b)
{
    int n = a.length();
    int m = b.length();

    #ifdef USE_STACK
    int res[2][m + 1]; // 2 filas
    #endif

    #ifdef USE_VECTOR
    vector< vector<int> > res( 2 , vector<int>(m+1) );
    #endif

    #ifdef USE_NEW
    int** res { new int*[2]{ } };
    for (int i{ 0 }; i < 2; i++)
    {
        // cada fila también debe ser inicializada con new
        res[i]  = new int[m+1]{ };
    }
    #endif

    for (int i = 0; i <= m; i++) {//relleno las filas      
        res[0][i] = i;
        res[1][i] = i;
    }

    for (int i = 1; i <= n; i++){
        res[i % 2][0] = i;
        for (int j = 1; j <= m; j++){
            if (a[i - 1] != b[j - 1]){
                res[i%2][j] = min(1 + res[(i - 1)%2][j - 1], min(1 + res[(i-1)%2][j], 1 + res[i%2][j - 1])); 
                //con el modulo sobreescribo los datos de la iteracion anterior a si solo necesito tener la info de  la fila de arriba y la misma fila a la cual pertenece  cada celda
            }
            else{
                res[i%2][j] = res[(i - 1)%2][j - 1];
            }
            
        }
    }
    int finalResult{ res[n%2][m] };

    #ifdef USE_NEW
    for (int i{ 0 }; i < 2; i++)
    {
        delete res[i];
    }
    delete res;
    #endif

    return finalResult;
    
}

/*
// based in the current chars at a[i], b[j] to be compared
int getCellVal(char a, char b, int left, int up, int diag)
{
    if(a == b)
    {
        return diag;
    }
    else
    {
        return 1 + min(left, min(up, diag));
    }
}
*/

#ifdef COMPUTE_CACHE2
// no c
//O(2*m)?? la verdad ni siquiera lo pense pero se ve como algo por el estilo
int EditDistCache2(string a, string b)
{
    int n = a.length();
    int m = b.length();
    
    #ifdef USE_STACK
    int temp[m]; // 1 fila de largo m (no se considera la columna 0 porque se puede inferir de i)
    #endif

    #ifdef USE_VECTOR
    vector<int> temp(m, 0);
    #endif

    #ifdef USE_NEW
    int* temp{ new int[m]{ } };

    for (int j = 1; j <= m; j++) {//relleno las filas      
        temp[j-1] = j;
    }
    #endif

    int left{ }; // (i,j-1)
    int up{ };   // (i-1,j)
    int diag{ }; // (i-1,j-1)
    int curr{ }; // (i,j)

    for (int i = 1; i <= n; i++){
        // obtain first value
        left = i;
        up = temp[0];
        diag = i-1;

        for (int j = 1; j < m; j++){
            if(a[i-1] == b[j-1])
            {
                curr = diag;
            }
            else
            {
                curr =  1 + min(left, min(up, diag));
            }
            // if the current value is equal to the value on top (stored in temp[j]), there's
            // no need to alter temp. Otherwise, we must overwrite the corresponding cell
            if(curr != up) 
            {
                temp[j-1] = curr;
            }
            left = curr;
            diag = up;
            up = temp[j];
        }
        // one last comparison 
        if(a[i-1] == b[m-1])
        {
            curr = diag;
        }
        else
        {
            curr =  1 + min(left, min(up, diag));
        }
        if(curr != up) 
        {
            temp[m-1] = curr;
        }
    }

    #ifdef USE_NEW
    delete[] temp;
    #endif

    return curr;
    
}
#endif


int EditDistBlock(string a, string b, int blockSize)
{
    int n = a.length();
    int m = b.length();

    // formula para obtener cantidad de bloques con fronteras compartidas 
    // dados M y B es ceil((M-1)/(B-1))
    int nM = (n%(blockSize-1))? n/(blockSize-1)+1: n/(blockSize-1);
    int mM = (m%(blockSize-1))? m/(blockSize-1)+1: m/(blockSize-1);

    // numero de filas y columnas efectivamente usadas en la tabla más adelante (son de a lo más largo blockSize)
    int filas{ };
    int columnas{ };

    // fronteraH almacenará los valores de las fronteras horizontales(sin solapar) de las
    // subtablas procesadas de izquierda a derecha. Hará de memoria secundaria, supongo
    
    #ifdef USE_STACK
    int fronteraH[blockSize*mM];
    #endif

    #ifdef USE_VECTOR
    vector<int> fronteraH(blockSize*mM, 0);
    #endif

    #ifdef USE_NEW
    int* fronteraH{ new int[blockSize*mM]{ } };
    #endif

    // tabla con largo x ancho = a un bloque de cache (?)
    int tabla[blockSize][blockSize];

    // inicializar frontera horizontal con los valores de 0 ... m (sin solapar)
    for(int jm{ }; jm < mM; jm++)
    {
        for(int j{ }; j < blockSize; j++)
        {
            fronteraH[jm*blockSize+j] = jm*(blockSize-1)+j;
        }

    }

    for(int im{ }; im < nM; im++)
    {
        for(int i{ }; i < blockSize; i++)
        {
            // inicializar primera columna de toda una hilera de bloques
            tabla[i][0] = im*(blockSize-1)+i;
        }
        for(int jm{ }; jm < mM; jm++)
        {
            // leer valores de frontera horizontal en memoria secundaria
            for(int j{ }; j < blockSize; j++)
            {
                tabla[0][j] = fronteraH[jm*blockSize+j];
            }
            // llenar subtabla
            // substrings correspondientes (notar que tienen largo blockSize-1 a lo más)
            string suba{ a.substr(im*(blockSize-1), blockSize-1) };
            string subb{ b.substr(jm*(blockSize-1), blockSize-1) };
            
            filas = suba.length()+1;
            columnas = subb.length()+1;

            for(int i{ 1 }; i < filas; i++)
            {
                for(int j{ 1 }; j < columnas; j++)
                {
                    if (suba[i - 1] != subb[j - 1])
                    {
                        tabla[i][j] = 1+ min(tabla[i - 1][j - 1], min(tabla[i - 1][j], tabla[i][j - 1])); 
                        // la casilla es determinado por el min entre las casillas aanteriores + 1
                        }
                        else
                        {
                            tabla[i][j] = tabla[i - 1][j - 1]; // si son iaguales los caracteres no hay costo por sobreescribir
                        }
                }

            }
            // fin edicion de tabla

            // guardar valores de frontera horizontal en memoria secundaria para la sgte iteración de im
            for(int j{ }; j < columnas; j++)
            {
                fronteraH[jm*blockSize+j] = tabla[filas-1][j];
            }
            // al finalizar, copiar frontera vertical resultante a la primera columna, así estará inicializada para la sgte
            // iteración
            for(int i{ }; i < filas; i++)
            {
                tabla[i][0] = tabla[i][columnas-1];
            }
            
        }

    }

    #ifdef USE_NEW
    delete[] fronteraH;
    #endif

    return tabla[filas-1][columnas-1];

}

// promedio
double mean(vector<int>& nums, size_t size)
{
    double sum{ };
    for(int i{ }; i < size; i++)
    {
        sum += nums[i];
    }
    return sum/size;

}

// desviación estándar
double stdev(vector<int>& nums, size_t size)
{
    double mu{ mean(nums, size) };
    double sigma{ };
    for(int i{ }; i < size; i++)
    {
        double prod{ nums[i] -mu };
        sigma += (prod*prod);
    }
    return sqrt(sigma/size);
}


static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
int stringLength = sizeof(alphanum) - 1;
char genRandom()  // Random string generator function.
{
    return alphanum[rand() % stringLength];
}

int main()
{

    cout << "Ingrese la cantidad de test seguido del largo de strings a probar y el tamano de los bloques de cache" << endl;
    int n, t, b;
    cin >> n >> t >> b;

    //Ejemplos individuales

    auto inicio = chrono::steady_clock::now();
    cout <<"Dynamic Edit dist entre banana y ananas es:" << EditDistDynamic("banana", "ananas") << endl;
    auto fin = chrono::steady_clock::now();
    double duracion = chrono::duration_cast<chrono::microseconds>(fin-inicio).count();
    cout <<"Me demore con el algoritmo dinamico: " << duracion << " microsegundos" << endl;

    auto inicio2 = chrono::steady_clock::now();
    cout <<"Cache Edit dist entre banana y ananas es:" << EditDistDynamic("banana", "ananas") << endl;
    auto fin2 = chrono::steady_clock::now();
    double duracion2 = chrono::duration_cast<chrono::microseconds>(fin2-inicio2).count();
    cout <<"Me demore con el algoritmo Cache: " << duracion2 << " microsegundos" << endl;

    // crear vector de resultados
    vector<int> resultadosDinamico{ };
    vector<int> resultadosCache{ };
    vector<int> resultadosBloque{ };
    int testCompletos{ };

    #ifdef COMPUTE_CACHE2
    vector<int> resultadosCache2{ };
    string header{ "len_string,len_bloque,n_tests,t_dinamico[μs],t_cache[μs],t_cache2[μs],t_particion[μs]" };
    #endif

    #ifndef COMPUTE_CACHE2
    string header{ "len_string,len_bloque,n_tests,t_dinamico[μs],t_cache[μs],t_particion[μs]" };
    #endif

    // guardar resultados en un txt
    string filename{ "time_results.csv" };

    // crear archivo y 
    ifstream f(filename.c_str());
    bool existe{f.good()};

    if(!existe)
    {
        std::ofstream outfile;
        outfile.open (filename);
        // colocar header si no existe
        outfile << header << '\n';
        outfile.close();
    }
    else
    {
        string firstLine;
        getline(f, firstLine);
        // fix outdated headers
        if(firstLine != header)
        {
            std::ofstream outfile;
            outfile.open (filename);
            outfile << header << '\n';
            outfile << f.rdbuf();
            outfile.close();
        }

    }

    f.close();

    std::ofstream outfile;
    // modo append
    outfile.open (filename, std::ios::app);

    try
    {
        
        while(n--){
            string source, target; //inicializacion de strings aleatorios
            for(unsigned int i = 0; i < t; i++){
                source += genRandom();
                target += genRandom();
            }
            //Test Distancia de edicion dinamico
            auto inicio = chrono::steady_clock::now();
            cout <<"Dynamic Edit dist: "+ source + " " + target + " es :" << EditDistDynamic(source, target) << endl;
            auto fin = chrono::steady_clock::now();
            double duracion = chrono::duration_cast<chrono::microseconds>(fin-inicio).count();
            cout <<"Me demore con el algoritmo dinamico: " << duracion << " microsegundos" << endl;

            //Test Distancia de edicion Cache
            auto inicio2 = chrono::steady_clock::now();
            cout <<"Cache Edit dist: "+ source + " " + target + " es :" << EditDistCache(source, target) << endl;
            auto fin2 = chrono::steady_clock::now();
            double duracion2 = chrono::duration_cast<chrono::microseconds>(fin2-inicio2).count();
            cout <<"Me demore con el algoritmo Cache: " << duracion2 << " microsegundos" << endl;

            #ifdef COMPUTE_CACHE2
            //Test Distancia de edicion Cache2
            auto inicio2a = chrono::steady_clock::now();
            cout <<"Cache Edit dist: "+ source + " " + target + " es :" << EditDistCache2(source, target) << endl;
            auto fin2a = chrono::steady_clock::now();
            double duracion2a = chrono::duration_cast<chrono::microseconds>(fin2a-inicio2a).count();
            cout <<"Me demore con el algoritmo Cache 2: " << duracion2a << " microsegundos" << endl;
            #endif

            //Test Distancia de edicion Particionado
            
            auto inicio3 = chrono::steady_clock::now();
            cout <<"Block Edit dist: "+ source + " " + target + " es :" << EditDistBlock(source, target, b) << endl;
            auto fin3 = chrono::steady_clock::now();
            double duracion3 = chrono::duration_cast<chrono::microseconds>(fin3-inicio3).count();
            cout <<"Me demore con el algoritmo Particionado: " << duracion3 << " microsegundos" << endl;

            cout << "Quedan " << n << " tests!\n";
            
            cout << endl;   
            
            resultadosDinamico.push_back(duracion);
            resultadosCache.push_back(duracion2);
            resultadosBloque.push_back(duracion3);

            #ifdef COMPUTE_CACHE2
            resultadosCache2.push_back(duracion2a);
            #endif

            testCompletos++;    

        }



    }
    // printear error sin terminar el programa aún
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    if(testCompletos == 0)
    {
        cout << "No lograron hacerse tests :(\n";
        outfile.close();
        exit (EXIT_FAILURE);
    }
    

    // calcular promedios
    double promDinamico{ mean(resultadosDinamico, testCompletos) };
    double promCache{ mean(resultadosCache, testCompletos) };
    double promBloque{ mean(resultadosBloque, testCompletos) };

    // calcular desviaciones estándar

    double stdevDinamico{ stdev(resultadosDinamico, testCompletos) };
    double stdevCache{ stdev(resultadosCache, testCompletos) }; 
    double stdevBloque{ stdev(resultadosBloque, testCompletos) };

    #ifdef COMPUTE_CACHE2
    double promCache2{ mean(resultadosCache2, testCompletos) };
    double stdevCache2{ stdev(resultadosCache2, testCompletos) };
    #endif

    cout <<"\nRESULTADOS FINALES" << "\n\n" << "Número de tests exitosos: " << testCompletos 
        << "\nTiempo[ms] para Algoritmo Dinámico -\tprom: " << promDinamico << " ;\tdevst: " << stdevDinamico
        << "\nTiempo[ms] para Algoritmo Caché -\tprom: " << promCache << " ;\tdevst: " << stdevCache
        #ifdef COMPUTE_CACHE2
        << "\nTiempo[ms] para Algoritmo Caché2 -\tprom: " << promCache2 << " ;\tdevst: " << stdevCache2
        #endif
        << "\nTiempo[ms] para Algoritmo Partición -\tprom: " << promBloque << " ;\tdevst: " << stdevBloque << '\n';
    
    // fila del csv con los tiempos
    outfile << t << ',' << b 
            << ',' << testCompletos 
            << ',' << promDinamico 
            << ',' << promCache 
            #ifdef COMPUTE_CACHE2
            << ',' << promCache2 
            #endif
            << ',' << promBloque << '\n';
    // cerrar archivo de tests
    outfile.close();

    
}