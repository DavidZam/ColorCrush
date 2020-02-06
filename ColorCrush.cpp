
// Practica 3: Color Crush
// Autores: David Zamora y Fernando Miñambres
// Laboratorio 1, puesto 16, [1ºB]
// Hemos hecho la parte extra que guarda las puntuaciones de usuario y las actualiza. 

#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
using namespace std;

//CONSTANTES, ENUMERADOS Y ESTRUCTURAS DE DATOS
const int DIMTABLERO = 8; //Dimension del tablero
const int NUMINTENTOS = 15; //Numero intentos maximo
const bool DEBUG = false; //Si esta a true se ejecuta system("pause"), si esta a false se ejecuta Sleep()
const int PAUSA = 300;//Numero de milisegundos que se ejecuta Sleep().
typedef enum { arriba, abajo, derecha, izquierda, error } tDireccion; //Enumerado de las direcciones del movimiento

typedef enum {	// Screen colors
	black,			//0
	dark_blue,		//1
	dark_green,		//2
	dark_cyan,		//3
	dark_red,		//4
	dark_magenta,	//5
	dark_yellow,	//6
	light_gray,		//7
	dark_gray,		//8
	light_blue,		//9
	light_green,	//10
	light_cyan,		//11
	light_red,		//12
	light_magenta,	//13
	light_yellow,	//14
	white			//15
} tColor;

typedef tColor tTablero[DIMTABLERO][DIMTABLERO];//Array bidimensional para representar el tablero.
typedef bool tEliminacion[DIMTABLERO][DIMTABLERO]; // Array bidimensional que indica qué celdas deben ser eliminadas

typedef struct {

	tTablero tablero;
	int intentosrestantes;
	int puntuacion;

} tJuego; //Estructura del juego.

typedef struct {

	int fila;
	int columna;
	tDireccion direccion;

} tMovimiento;// Estructura del movimiento

//DECLARACION DE PROTOTIPOS
void setColor(tColor color); //Procedimiento que asigna colores a las fichas del tablero
void mostrarTablero(const tJuego juego);//Procedimiento para mostrar el tablero.Dividido en tres partes:
void mostrarTableroArriba(const tJuego juego);//Muestra la parte superior del tablero
void mostrarTableroCuerpo(const tJuego juego);//Muestra el cuerpo del tablero
void mostrarTableroAbajo();//Muestra la parte inferior del tablero
void generarTablero(tJuego &juego);//Procedimiento para generar un tablero aleatorio
tColor colorAleatorio();//Funcion que genera colores a partir de numeros aleatorios
void procesarTablero(tJuego &juego, bool &cambio);//Procedimiento para procesar el tablero.Dividido en dos partes:
void procesarTableroHorizontal(tJuego &juego, tEliminacion fichas, bool &cambio);//Procesa el tablero por filas
void procesarTableroVertical(tJuego &juego, tEliminacion fichas, bool &cambio);//Procesa el tablero por columnas
void eliminarFichas(tJuego &juego, tEliminacion fichas);//Procedimiento para eliminar grupos de 3 o mas fichas consecutivas del mismo color
void processLoop(tJuego &juego);//Bucle para procesar el tablero mientras existan grupos de fichas
void dropLoop(tJuego &juego);//Bucle para rellenar el tablero mientras existan huecos libres
void caidaFichas(tJuego &juego, bool &caida);//Procedimiento para rellenar los espacios libres del tablero
int puntuacion(int grupo_fichas);//Funcion que devuelve la puntuacion obtenida en base a los grupos de fichas conseguidos.
void jugar(tJuego &juego);//Procedimiento principal del juego.
tDireccion letraAdireccion(char letra_dir);//Funcion que transforma una letra en una direccion
bool tryMove(tJuego &juego, tMovimiento move);//Funcion que comprueba la validez del movimiento introducido por el usuario
void incremento(tDireccion direccion, int &Incfila, int &Incolumna);//Procedimiento para aumentar o disminuir la fila o columna en base a la direccion del movimiento
void exchange(tJuego &juego, tMovimiento move, bool &correcto);//Procedimiento que intercambia las fichas del tablero
bool comprobarCelda(const tJuego juego, int fila, int columna);//Funcion que busca la creacion de grupos de fichas tras un movimiento. Dividido en dos partes:
bool comprobarCeldaVertical(const tJuego juego, int fila, int columna);//Funcion para buscar grupos de fichas hacia arriba y hacia abajo
bool comprobarCeldaHorizontal(const tJuego juego, int fila, int columna);//Funcion para buscar grupos de fichas hacia izquierda y hacia derecha
void guardarPuntuaciones(string nombre, int puntos);//Procedimiento que guarda los usuarios y sus puntuaciones.

//SUBPORGRAMAS IMPLEMENTADOS POR EL PROGRAMADOR
int main() {
	tJuego juego;
	srand(time(NULL));
	jugar(juego);

	return 0;

} // main

void setColor(tColor color) {

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color);

} //setColor()

void mostrarTablero(const tJuego juego) {

	if (!DEBUG) {
		system("cls");
		mostrarTableroArriba(juego);
		mostrarTableroCuerpo(juego);
		Sleep(PAUSA);
	}
	else {
		mostrarTableroArriba(juego);
		mostrarTableroCuerpo(juego);
		system("pause");
	} // else

} // mostrarTablero()

void mostrarTableroArriba(const tJuego juego) {
	setColor(white);
	cout << "      Intentos: " << juego.intentosrestantes << "    Puntos: " << juego.puntuacion << endl;
	cout << "  " << char(218);
	for (int col = 0; col < DIMTABLERO - 1; col++) {
		cout << char(196) << char(196) << char(196) << char(194);
	} // for

	cout << char(196) << char(196) << char(196) << char(191);
	cout << endl;

} // mostrarTableroArriba

void mostrarTableroCuerpo(const tJuego juego) {

	for (int fila = DIMTABLERO - 1; fila >= 0; fila--) {
		cout << fila + 1 << " ";
		for (int col = 0; col < DIMTABLERO; col++) {
			cout << char(179);
			setColor(juego.tablero[fila][col]);
			cout << char(219) << char(219) << char(219);
			setColor(white);
		} // for
		cout << char(179) << endl;
		cout << "  ";
//Cuando la fila sea la ultima, creamos la parte inferior del tablero
		if (fila == 0){ 
			mostrarTableroAbajo();
		}
		else {
			cout << char(195);
			for (int col = 0; col < DIMTABLERO - 1; col++) {
				cout << char(196) << char(196) << char(196) << char(197);
			} // for
			cout << char(196) << char(196) << char(196) << char(180) << endl;
		}//else
	} // for

} // mostrarTableroCuerpo

void mostrarTableroAbajo() {

	cout << char(192);
	for (int col = 0; col < DIMTABLERO - 1; col++) {
		cout << char(196) << char(196) << char(196) << char(193);
	} // for
	cout << char(196) << char(196) << char(196) << char(217) << endl;

	cout << "   ";
	for (int col = 1; col <= DIMTABLERO; col++) {
		cout << " " << col << "  ";
	} // for
	cout << endl;

} // mostrarTableroAbajo

void generarTablero(tJuego &juego) {

	juego.intentosrestantes = NUMINTENTOS; //Incializamos los intentos al maximo de intentos permitidos.
	juego.puntuacion = 0;//Inicializamos la puntuacion a 0.
	for (int fila = 0; fila < DIMTABLERO; fila++) {//Bucle para ir asignando un numero a cada celda del tablero
		for (int col = 0; col < DIMTABLERO; col++) {
			juego.tablero[fila][col] = colorAleatorio();
		} // for
	} // for

} // generarTablero()

tColor colorAleatorio() {

	int numero;
	tColor color;
	numero = rand() % 4;

	switch (numero) { //Switch que hace corresponder un color al numero generado aleatoriamente

	case 0: color = light_red; break;
	case 1: color = light_yellow; break;
	case 2: color = light_blue; break;
	case 3: color = light_green; break;

	} // switch

	return color;

} // aleatorioColores()

void procesarTablero(tJuego &juego, bool &cambio) {

	tEliminacion fichas = { false }; //Inicializamos el array booleano a false.
	cambio = false; //Inicializamos cambio a false;
	procesarTableroHorizontal(juego, fichas, cambio);
	procesarTableroVertical(juego, fichas, cambio);
	eliminarFichas(juego, fichas);
}// procesarTablero()

void procesarTableroHorizontal(tJuego &juego, tEliminacion fichas, bool &cambio) {
	int contador = 1;
	for (int fila = 0; fila < DIMTABLERO; fila++) {
		tColor colorantiguo = black; //Creamos una variable tColor para saber cual es el ultimo color recorrido.
		for (int col = 0; col < DIMTABLERO; col++) {

			if (juego.tablero[fila][col] == colorantiguo) {//Comparamos el color antiguo con el actual.
				contador++;
				//Caso especial ultima columna
				if ((contador >= 3) && (col == DIMTABLERO - 1)){
					for (int grupofichas = col; grupofichas >(col - contador); grupofichas--) { //Bucle que pone a true en el array booleano los grupos de fichas conseguidos
						fichas[fila][grupofichas] = true;
					} // for
					cambio = true;
					juego.puntuacion += puntuacion(contador);//contabilizamos la puntuacion
					contador = 1;
				}//if
			}
			else {
				if (contador >= 3) {
					cambio = true;
					juego.puntuacion += puntuacion(contador);//contabilizamos la puntuacion
					for (int grupofichas = col - 1; grupofichas > (col - 1 - contador); grupofichas--) {//Bucle que pone a true en el array booleano los grupos de fichas conseguidos
						fichas[fila][grupofichas] = true;
					} // for
				} // if
				colorantiguo = juego.tablero[fila][col];
				contador = 1;
			}//else
		}//for
		contador = 1;
	}//for

	//procesarTableroVertical(juego, fichas, cambio);

}// ProcesarTableroHorizontal()

void procesarTableroVertical(tJuego &juego, tEliminacion fichas, bool &cambio) {

	int contador = 1;
	for (int col = 0; col < DIMTABLERO; col++) {
		tColor colorantiguo = black;
		for (int fila = 0; fila < DIMTABLERO; fila++) {
			if (juego.tablero[fila][col] == colorantiguo) {
				contador++;
				//Caso especial ultima fila
				if ((contador >= 3) && (fila == DIMTABLERO - 1)){
					for (int grupofichas = fila; grupofichas >(fila - contador); grupofichas--) {//Bucle que pone a true en el array booleano los grupos de fichas conseguidos
						fichas[grupofichas][col] = true;
					} // for
					cambio = true;
					juego.puntuacion += puntuacion(contador);//contabilizamos la puntuacion
					contador = 1;
				}//if
			}
			else {
				if (contador >= 3) {
					cambio = true;
					juego.puntuacion += puntuacion(contador);//contabilizamos la puntuacion
					for (int grupofichas = contador; grupofichas > 0; grupofichas--) {//Bucle que pone a true en el array booleano los grupos de fichas conseguidos
						fichas[fila - grupofichas][col] = true;
					} // for
				} // if
				colorantiguo = juego.tablero[fila][col];
				contador = 1;
			}//else
		}//for
		contador = 1;
	}//for

}// ProcesarTableroVertical()

void eliminarFichas(tJuego &juego, tEliminacion fichas) {
	Sleep(PAUSA);

	for (int fila = 0; fila < DIMTABLERO; fila++){ //Bucle que pone a black las celdas que en el array booleano esten a true.
		for (int col = 0; col < DIMTABLERO; col++){
			if (fichas[fila][col] == true){

				juego.tablero[fila][col] = black;

			}//if
		}//for
	}//for
	cout << endl;
} // EliminarFichas()

void processLoop(tJuego &juego) {

	bool cambio = true;

	while (cambio){ //Mientras haya habido algun cambio en el tablero...

		procesarTablero(juego, cambio);

		if (cambio) { //Si hay cambio:

			mostrarTablero(juego);

			dropLoop(juego);
		}//if
	}//while

}//processLoop()

void dropLoop(tJuego &juego) {
	bool dropped = true;

	while (dropped) { //Mientras haya caida de fichas...

		caidaFichas(juego, dropped);

		if (dropped){ // Si ha habido caida de fichas

			mostrarTablero(juego);

		}//if

	}//while
}//dropLoop()

void caidaFichas(tJuego &juego, bool &caida){

	caida = false; //Inicializamos caida a false.

	for (int col = 0; col < DIMTABLERO; col++){ //Bucle que intercambia la celda a negro por la celda de arriba
		for (int fila = 0; fila < DIMTABLERO - 1; fila++){
			if (juego.tablero[fila][col] == black){

				juego.tablero[fila][col] = juego.tablero[fila + 1][col];
				juego.tablero[fila + 1][col] = black;
				caida = true;

			}//if

		}//for
	}//for

	for (int col = 0; col < DIMTABLERO; col++){ //Bucle que genera una ficha aleatoria si hay una celda a negro en la fila 8
		for (int fila = DIMTABLERO - 1; fila < DIMTABLERO; fila++){

			if (juego.tablero[fila][col] == black){
				juego.tablero[fila][col] = colorAleatorio();
				caida = true;
			}//if
		}//for
	}//for
	cout << endl;

}//rellenarTablero()

int puntuacion(int grupo_fichas) {

	int puntuacion = 0; //Inicializamos puntuacion a 0.

	if (grupo_fichas == 3){
		puntuacion = 1 * grupo_fichas;
	}
	else
	if (grupo_fichas == 4){

		puntuacion = 2 * grupo_fichas;

	}
	else
	if (grupo_fichas >= 5){

		puntuacion = 15;
	}//if

	return puntuacion;

}//puntuacion()

void jugar(tJuego &juego) {

	bool fin = false;// Variable que indica la salida del juego.
	bool ok = false; // Variable que indica si el juego se va procesando correctamente.
	tMovimiento movimiento;//Variable para el movimiento.
	char letra_dir; //Variable que corresponde a la letra introducida por el usuario.
	string nombre;//Variable para el nombre del jugador
	//Solicitamos el nombre del jugador

	cout << "Bienvenido a Color Crush, introduzca su nombre: ";
	cin >> nombre;

	generarTablero(juego);
	mostrarTablero(juego);
	processLoop(juego);

	while (!fin) { //Mientras no lleguemos a fin.

		if (juego.intentosrestantes < 1) { // Si no quedan intentos...

			fin = true;

		}
		else {
			//Pedimos al usuario que introduzca un movimiento.
			cout << "Fila, columna y letra de direccion (A, B, I o D; 0 para cancelar): ";
			cin >> movimiento.fila;
			//Si el usuario no ha introducido un 0 para salir...
			if (movimiento.fila == 0) {

				fin = true;

			}
			else {
				//Restamos 1 a la fila para ajustarlo a la dimension del array
				movimiento.fila -= 1;
				//Leemos la columna
				cin >> movimiento.columna;
				//Restamos 1 a la columna para ajustarlo a la dimension del array
				movimiento.columna -= 1;
				//Leemos la direccion y la pasamos a tMovimiento.
				cin >> letra_dir;
				movimiento.direccion = letraAdireccion(letra_dir);
				//Comprobamos si el movimiento es valido:
				ok = tryMove(juego, movimiento);
				//Si no es valido: informamos del error y solicitamos un nuevo movimiento sin contar intento.
				if (!ok){

					cout << "El movimiento no se realizado correctamente" << endl;
					system("pause");
					mostrarTablero(juego);
				}
				else{ //Si es valido:Ejecutamos el movimiento.
					//Restamos un intento
					juego.intentosrestantes--;
					//Mostramos el tablero tras el movimiento
					mostrarTablero(juego);
					//Procesamos el tablero.
					processLoop(juego);
				}//else
			} //else
		}//else
	}//while
	//Guardamos las puntuaciones en el archivo de texto.
	guardarPuntuaciones(nombre, juego.puntuacion);
	//Mostramos un mensaje de final del juego.
	cout << " FIN DEL JUEGO. Tus puntos: " << juego.puntuacion << endl;
	system("pause");
}//void jugar

bool tryMove(tJuego &juego, tMovimiento move) {
	int Incfila = 0, Incolumna = 0;
	bool ok = false;
	//Comprobar si fila y columna FUENTE esta dentro del tablero:
	if (move.fila >= 0 && move.fila <= DIMTABLERO - 1) ok = true;
	if (move.columna >= 0 && move.columna <= DIMTABLERO - 1) ok = true;
	//Comprobar que la direccion es valida
	if (move.direccion == error) {
		ok = false;
	}
	else
		ok = true;
	//Si la celda FUENTE es valida, miramos la DESTINO.
	if (ok){
		incremento(move.direccion, Incfila, Incolumna);
		//Comprobar si la fila y la columnma estan dentro del tablero despues del incremento:
		if (move.fila + Incfila >= 0 && move.fila + Incfila <= DIMTABLERO - 1) ok = true;
		if (move.columna + Incolumna >= 0 && move.columna + Incolumna <= DIMTABLERO - 1) ok = true;
		//Si todo es correcto, pasamos a intercambiar las dos celdas.
		if (ok) {
			exchange(juego, move, ok);
		}//if
	}//if
	return ok;
}// bool tryMove()

tDireccion letraAdireccion(char letra_dir) {

	tDireccion direccion;

	switch (toupper(letra_dir)){ // Switch para saber la direccion del movimiento a partir de la letra introducida( sea minuscula o mayuscula).

	case 'A': direccion = arriba; break;
	case 'B': direccion = abajo; break;
	case 'I': direccion = izquierda; break;
	case 'D': direccion = derecha; break;
	default: direccion = error; break;

	}//switch

	return direccion;

} //letraAdireccion

void incremento(tDireccion direccion, int &Incfila, int &Incolumna){
	Incfila = 0;
	Incolumna = 0;

	switch (direccion) {

	case arriba: Incfila++; break;
	case abajo: Incfila--; break;
	case izquierda: Incolumna--; break;
	case derecha:Incolumna++; break;

	} //switch

} // Incremento()

void exchange(tJuego &juego, tMovimiento move, bool &correcto){
	int Incfila = 0;
	int Incolumna = 0;
	correcto = false;
	tColor aux;

	//Localizar¡mos la ficha DESTINO
	incremento(move.direccion, Incfila, Incolumna);
	aux = juego.tablero[move.fila + Incfila][move.columna + Incolumna];
	//Intercambiamos las fichas.
	juego.tablero[move.fila + Incfila][move.columna + Incolumna] = juego.tablero[move.fila][move.columna];
	juego.tablero[move.fila][move.columna] = aux;
	//Comprobamos si hay algun grupo de fichas creado.
	if (!(correcto = comprobarCelda(juego, move.fila, move.columna))){
		correcto = comprobarCelda(juego, move.fila + Incfila, move.columna + Incolumna);
	}
	// si correcto = false
	if (!correcto){
		//Deshacemos el cambio.
		aux = juego.tablero[move.fila][move.columna];
		juego.tablero[move.fila][move.columna] = juego.tablero[move.fila + (Incfila)][move.columna + (Incolumna)];
		juego.tablero[move.fila + (Incfila)][move.columna + (Incolumna)] = aux;
		//Informamos del error.
		cout << "No se ha formado ningun grupo de fichas." << endl;
	}//if
}// void exchange();

bool comprobarCelda(const tJuego juego, int fila, int columna) {

	bool ok = false;
	int contador = 1;
	//Si no encontramos ningun grupo en vertical, pasamos a mirar en horizontal.
	if (!(ok = comprobarCeldaVertical(juego, fila, columna))){

		ok = comprobarCeldaHorizontal(juego, fila, columna);

	}//if

	return ok;

}//bool match()

bool comprobarCeldaVertical(const tJuego juego, int fila, int columna){

	bool ok = false;
	int contador = 1, prevRow, nextRow;
	prevRow = fila - 1;

	//Contamos celdas del mismo color hacia abajo.
	while (juego.tablero[prevRow][columna] == juego.tablero[fila][columna] && prevRow >= 0){

		contador++;
		prevRow--;

	}// while

	nextRow = fila + 1;
	//Contamos celdas del mismo color hacia arriba.
	while (juego.tablero[nextRow][columna] == juego.tablero[fila][columna] && nextRow <= DIMTABLERO - 1){

		contador++;
		nextRow++;

	}//while
	//Si encontramos un grupo de 3 fichas o mas.
	if (contador >= 3) {
		ok = true;
	} // if

	return ok;
}//bool matchVertical

bool comprobarCeldaHorizontal(const tJuego juego, int fila, int columna){

	bool ok = false;
	int contador = 1, prevCol, nextCol;
	prevCol = columna - 1;

	//Contamos celdas del mismo color hacia la izquierda.
	while (juego.tablero[fila][prevCol] == juego.tablero[fila][columna] && prevCol >= 0){

		contador++;
		prevCol--;

	}// while

	nextCol = columna + 1;
	//Contamos celdas del mismo color hacia la derecha.
	while (juego.tablero[fila][nextCol] == juego.tablero[fila][columna] && nextCol <= DIMTABLERO - 1){

		contador++;
		nextCol++;

	}//while
	//Si encontramos un grupo de 3 o mas fichas
	if (contador >= 3){
		ok = true;
	}

	return ok;
}//bool matchVertical

void guardarPuntuaciones(string nombre, int puntos){

	ofstream escribir;
	ifstream archivo;
	bool encontrado = false;
	string name = "";
	string centinela = "X";
	int puntuacion = 0;
	//Abrimos el archivo de lectura
	archivo.open("puntuaciones.txt");
	//Si no existe, lo crea
	if(!archivo.is_open()){
		//Abrimos el archivo de escritura
		escribir.open("puntuaciones.txt");
		//Escribimos el nombre y la puntuacion y el centinela
		escribir << nombre << " ";
		escribir << puntuacion + puntos << endl;
		escribir << centinela << endl;
		//Cerramos el archivo
		escribir.close();
	}
	else {
		//Leemos el primer nombre en el archivo
		archivo >> name;
		//Abrimos el archivo de escritura
		escribir.open("puntuaciones.txt");
		//Mientras no lleguemos al centinela ni encontremos el mismo nombre...
		while (name != centinela && !encontrado){
			//Si el nombre es el mismo:
			if (name == nombre){
				encontrado = true;
			}
			else{
				//Escribimos el nombre y la puntuacion.
				escribir << name << " ";
				archivo >> puntuacion;
				escribir << puntuacion << endl;
			}//else
			//Leemos el siguiente nombre...
			archivo >> name;
		}//while
		//Si el nombre ya existia, actualizamos el fichero..
		if (encontrado){
			//Si el nombre es el mismo:
			if (name == nombre){

				escribir << name << " ";
				archivo >> puntuacion;
				//Sumamos la puntuacion
				escribir << puntuacion + puntos << endl;
				//leemos el siguiente nombre
				archivo >> name;

				while (name != centinela){

					escribir << name << " ";
					archivo >> puntuacion;
					escribir << puntuacion << endl;
					//Leemos el siguiente nombre
					archivo >> name;
				}//while
				escribir << centinela << endl;
			}//if
		}
		else{
			//Escribimos la nueva informacion
			escribir << nombre << " ";
			escribir << puntos << endl;
			escribir << centinela << endl;
		}//else
		//Cerramos ambos archivos.
		archivo.close();
		escribir.close();
	}//else

}// void guardarPuntuaciones();



