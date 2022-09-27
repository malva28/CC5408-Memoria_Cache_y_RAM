#include <bits/stdc++.h>
using namespace std;

// El algoritmo tambien funciona en strings de tamaño distintos
//  O(n*m) ???
int EditDistDynamic(string a, string b) // Algoritmo-1 Distancia de Edicion Programacion dinamica
{
    int filas = a.length();
    int columnas = b.length();
    int res[filas + 1][columnas + 1] = {{0}}; // grilla de n+1, m+1
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
    return res[filas][columnas];
}

//no funciona en strigns de largos distinto:c TODO: revisar pq'
//O(2*m)?? la verdad ni siquiera lo pense pero se ve como algo por el estilo
int EditDistCache(string a, string b)
{
    int n = a.length();
    int m = b.length();
    int res[2][m + 1]; // 2 filas
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
    return res[n%2][m];
    
}

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

// no c
//O(2*m)?? la verdad ni siquiera lo pense pero se ve como algo por el estilo
int EditDistCache2(string a, string b)
{
    int n = a.length();
    int m = b.length();
    int temp[m]; // 1 fila de largo m (no se considera la columna 0 porque se puede inferir de i)

    for (int j = 1; j <= m; j++) {//relleno las filas      
        temp[j-1] = j;
    }

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
            curr = getCellVal(a[i-1], b[j-1], left, up, diag);
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
        curr = getCellVal(a[i-1], b[m-1], left, up, diag);
        if(curr != up) 
        {
            temp[m-1] = curr;
        }
    }
    return curr;
    
}


int EditDistBlock(string a, string b, int blockSize)
{
    int n = a.length();
    int m = b.length();

    // formula para obtener cantidad de bloques con fronteras compartidas 
    // dados M y B es ceil((M-1)/(B-1))
    int nM = (n%(blockSize-1))? n/(blockSize-1)+1: n/(blockSize-1);
    int mM = (m%(blockSize-1))? m/(blockSize-1)+1: m/(blockSize-1);

    // numero de filas y columnas efectivamente usadas en la tabla más adelante (son de a lo más largo blockSize)
    int filas;
    int columnas;

    // fronteraH almacenará los valores de las fronteras horizontales(sin solapar) de las
    // subtablas procesadas de izquierda a derecha. Hará de memoria secundaria, supongo
    int fronteraH[blockSize*mM];

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
    return tabla[filas-1][columnas-1];

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
    while(n--){
        string source, target;
        for(unsigned int i = 0; i < t; i++){
            source += genRandom();
            target += genRandom();
        }
        cout <<"Dynamic Edit dist: "+ source + " " + target + " es :" << EditDistDynamic(source, target) << endl;
        cout <<"Cache Edit dist: "+ source + " " + target + " es :" << EditDistCache2(source, target) << endl;
        cout <<"Cache Edit dist: "+ source + " " + target + " es :" << EditDistBlock(source, target, b) << endl;

    }



    cout << EditDistDynamic("banana", "ananas") << endl;
    cout << EditDistCache("banana", "ananas") << endl;
    cout << "Adios" << endl;
}