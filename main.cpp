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
                res[i][j] = min(1 + res[i - 1][j - 1], min(1 + res[i - 1][j], 1 + res[i][j - 1])); // la casilla es determinado por el min entre las casillas aanteriores + 1
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

static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
int stringLength = sizeof(alphanum) - 1;
char genRandom()  // Random string generator function.
{
    return alphanum[rand() % stringLength];
}

int main()
{
    cout << "Ingrese la cantidad de test seguido del largo de strings a probar" << endl;
    int n, t;
    cin >> n >> t;
    while(n--){
        string source, target;
        for(unsigned int i = 0; i < t; i++){
            source += genRandom();
            target += genRandom();
        }
        cout <<"Dynamic Edit dist: "+ source + " " + target + " es :" << EditDistDynamic(source, target) << endl;
        cout <<"Cache Edit dist: "+ source + " " + target + " es :" << EditDistCache(source, target) << endl;

    }



    cout << EditDistDynamic("banana", "ananas") << endl;
    cout << EditDistCache("banana", "ananas") << endl;
    cout << "Adios" << endl;
}