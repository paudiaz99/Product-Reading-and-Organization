/****************************************************************************
*
* @Propòsit: Creació d'un sistema que permet llegir una serie de fitxers on estàn escrits diferents productes amb el seu preu i stock respectivament
* @Autor/s: Pau Díaz Cuesta
* @Data de creació: 22 de Febrer de 2022
* @Data de l’última modificació: 25 de Febrer de 2022
*
* ***************************************************************************/

// Llibreries del sistema
#include <stdio.h>
#include <string.h>

// Constants
#define MAX_STRING_LENGTH 70

// Tipus Propis
typedef struct {
		char name[MAX_STRING_LENGTH];
		int stock;
		float price;
}Product;

/*********************************************** 
*
* @Finalitat: Afegir l'extenstió .txt o .bin depenent del cas
* @Paràmetres: in: file_num = numero que determina quin supplier escull l'usuari
*			   in: file_name = nom del archiu al qual se li afegirà l'extensió
* @Retorn: ----
*
* **********************************************/
void findFileName(int file_num, char file_name[]){
	char trash; 				// Variable per netejar el buffer

	// Miro quina opció de supplier ha escollit l'usuari i en funció d'aquesta afegeixo al nom .txt o .bin
	if(file_num != 3){	
		printf("\nType the name of the file (without '.txt'): ");
		scanf("%s", file_name);
		scanf("%c", &trash);
		strcat(file_name, ".txt");	
	}
	else{
		printf("\nType the name of the file (without '.bin'): ");
		scanf("%s", file_name);
		scanf("%c", &trash);
		strcat(file_name, ".bin");
	}
}

/*********************************************** 
*
* @Finalitat: Indicar si s'ha obert correctament el fitxer
* @Paràmetres: in: file_name = nom del fitxer al qual se li afegirà l'extensió
*				in: f = variable del fitxer obert
* @Retorn: Retornara una variable flag que determinarà si el fitxer és vàlid (1 o 0)
*
* **********************************************/
int validFile(char file_name[], FILE *f){
	int valid = 0;					// Variable que determina si el file és correcte, la qual serà retornada

	// Comprovo si sha obert correctament el fitxer
	if (f == NULL){
		valid = 0;
		printf("\nERROR: Can't open file '%s'\n", file_name);
	}
	else{
		// En cas afirmatiu, el fitxer és valid
		valid = 1;
	}
	return(valid);
}

/*********************************************** 
*
* @Finalitat: Indicar si el producte és valid per ser guardat o en cas de estar repetit, realitzar algunes modificacions
* @Paràmetres: in: prod_price = preu del producte llegit
*			   in: prod_stock = stock del producte llegit
*			   in: prod_num = numero de productes guardats actualment
*			   in: prod_name = nom del producte llegit
*			   in: p = array del tipus Product on estàn tots els productes guardats 	
* @Retorn: Retornarà una variable flag que determinarà si el producte és vàlid (1 o 0)
*
* **********************************************/
int validProduct(int prod_price, int prod_stock, int prod_num, char prod_name[], Product p[]){
	int valid = 1;				// Variable flag que indicarà si el producte és correcte o no per ser guardat
	int i = 0;					// Variable per als bucles for
	
	// Faig un bucle per determinar "no valid" tots els productes que continguin una @
	for(i=0;i<strlen(prod_name);i++){
		if (prod_name[i] == '@'){
			valid = 0;
		}
	}

	// Faig un bucle per trobar si el producte ha estat repetit
	for(i=0;i<prod_num;i++){
			if(strcmp(p[i].name, prod_name) == 0 && prod_stock != 0){
				valid = 0;
				// Substitueixo el preu del producte repetit per el més alt
				if(p[i].price < prod_price){
					p[i].price = prod_price;
				}
				// Afegeixo el stock al producte repetit
				p[i].stock = p[i].stock +  prod_stock;
			}
	}
	// Comprovo si el stock del producte es 0
	if(prod_stock == 0){
		// En cas afirmatiu, el producte llegit passa a ser no valid
		valid = 0;
	}

	return valid;
}
/*********************************************** 
*
* @Finalitat: Carregar els productes del supplier 1
* @Paràmetres: in: p = cadena de tipus Product on es guardaràn els productes vàlids
*			   in: f = variable del fitxer obert
* 			   in: prod_num = numero de productes que hi ha guardats a p[]
* @Retorn: Retornarà el numero de productes guardats actualitzat
*
* **********************************************/
int loadSupplier1(Product p[], FILE *f, int prod_num){
	int stock = 0;														// Variables de les tres característiques dels productes
	float price = 0;
	char prod_name[MAX_STRING_LENGTH];	
	char trash;															// Variable per a netejar espais
	int i = prod_num;													// Varibale i inicialitzada a prod_num la qual serà utilitzada per guardar els productes
	int valid = 0;														// Variable que determinarà si el producte és valid per ser guardat

		// Escanejo el fitxer segons el format
		fscanf(f, "%d", &stock);
		fscanf(f, "%f", &price);
		fscanf(f, "%c", &trash);
		fgets(prod_name, MAX_STRING_LENGTH - 1, f);
		prod_name[strlen(prod_name)-1] = '\0';
		// Faig un bucle fins que s'arribi al final del fitxer
		while(!feof(f)){
			// Comprovo si la inormació llegida és vàlida amb la  valid Product
			valid = validProduct(price, stock, prod_num, prod_name, p);
			// Si es vàlid, guardo la informació llegida
			if(valid){
				p[i].stock = stock;
				p[i].price = price;
				strcpy(p[i].name, prod_name);
				i++;
			}
			// Escanejo el fitxer segons el format
			fscanf(f, "%d", &stock);
			fscanf(f, "%f", &price);
			fscanf(f, "%c", &trash);
			fgets(prod_name, MAX_STRING_LENGTH - 1, f);
			prod_name[strlen(prod_name)-1] = '\0';
		}
	return(i);
}
/*********************************************** 
*
* @Finalitat: Carregar els productes del supplier 2
* @Paràmetres: in: p = cadena de tipus Product on es guardaràn els productes vàlids
*			   in: f = variable del fitxer obert
* 			   in: prod_num = numero de productes que hi ha guardats a p[]
* @Retorn: Retornarà el numero de productes guardats actualitzat
*
* **********************************************/
int loadSupplier2(Product p[], FILE *f, int prod_num){
	int i = prod_num;														// Varibale i inicialitzada a prod_num la qual serà utilitzada per guardar els productes
	int stock = 0;															// Variables de les tres característiques dels productes
	char prod_name[MAX_STRING_LENGTH];
	float price = 0;		
	char trash_str[MAX_STRING_LENGTH];										// Variable per netejar paraules no desitjades
	char trash;																// Variable per netejar espais
	int valid = 0;															// Variable que determinarà si el producte és valid per ser guardat

	// Escanejo el fitxer segons el format
	fscanf(f, "%s", trash_str);
	fscanf(f, "%c", &trash);
	fscanf(f, "%s", trash_str);
	fscanf(f, "%c", &trash);
	fgets(prod_name, MAX_STRING_LENGTH - 1, f);
	prod_name[strlen(prod_name)-1] = '\0';
	fscanf(f, "%s", trash_str);
	fscanf(f, "%f", &price);
	fscanf(f, "%s", trash_str);
	fscanf(f, "%s", trash_str);
	// Faig un bucle fins que s'arribi al final del fitxer
	while(!feof(f)){
		// Comprovo si la inormació llegida és vàlida amb la  valid Product
		valid = validProduct(price, stock, prod_num, prod_name, p);
		// Si es vàlid, guardo la informació llegida
		if(valid){
			p[i].stock = stock;
			p[i].price = price;
			strcpy(p[i].name, prod_name);
			i++;
		}
		// Escanejo el fitxer segons el format
		fscanf(f, "%s", trash_str);
		fscanf(f, "%c", &trash);
		fscanf(f, "%s", trash_str);
		fscanf(f, "%c", &trash);
		fgets(prod_name, MAX_STRING_LENGTH - 1, f);
		prod_name[strlen(prod_name)-1] = '\0';
		fscanf(f, "%s", trash_str);
		fscanf(f, "%f", &price);
		fscanf(f, "%s", trash_str);
		fscanf(f, "%s", trash_str);
		fscanf(f, "%d", &stock);
	}
	
	return(i);
}

/*********************************************** 
*
* @Finalitat: Carregar els productes del supplier 3
* @Paràmetres: in: p = cadena de tipus Product on es guardaràn els productes vàlids
*			   in: f = variable del fitxer obert
* 			   in: prod_num = numero de productes que hi ha guardats a p[]
* @Retorn: Retornarà el numero de productes guardats actualitzat
*
* **********************************************/
int loadSupplier3(Product p[], FILE *f, int prod_num){
	int i = prod_num;			// Varibale i inicialitzada a prod_num la qual serà utilitzada per guardar els productes
	Product p_read;				// Variable de tipus Product on es guardarà la informació llegida del fitxer binari
	int valid = 0;				// Variable que determinarà si el producte és valid per ser guardat

	// Escanejo el fitxer amb fread
	fread(&p_read, sizeof(Product), 1, f);

	// Faig un bucle fins que s'arribi al final del fitxer
	while(!feof(f)){

		// Comprovo si el producte és valid amb la funció validProduct
		valid = validProduct(p_read.price, p_read.stock, prod_num, p_read.name, p);
		// En cas afirmatiu guardo a p la informació llegida
		if(valid){
			strcpy(p[i].name, p_read.name);
			p[i].stock = p_read.stock;
			p[i].price = p_read.price;
			i++;
		}
	
		// Torno a escanejar el fitxer amb fread
		fread(&p_read, sizeof(Product), 1, f);
	}
	
	return(i);
}

/*********************************************** 
*
* @Finalitat: Demanar al usuari el nom d'un producte i buscar si aquest està ha estat guardat
* @Paràmetres: in: p = array del tipus Product on estàn guardats tots els Productes
* 			   in: prod_num = numero de productes que hi ha guardats a p[]
* @Retorn: ----
*
* **********************************************/
void findProdName(Product p[], int prod_num){
	
	int i = 0;										// Variable per als bucles for
	int prod_found = 0;								// Variable que serivrà de flag per determinar si el producte s'ha trobat o no (1 o 0)
	char prod_name[MAX_STRING_LENGTH];				// Nom del producte que es buscarà
	char trash;										// Variable per netejar el buffer

	// Demano al usuari el nom del producte que vol cercar l'usauri
	printf("\nProduct name?: ");

	// Netejo el buffer per evitar errors
	scanf("%c", &trash);

	// Escanejo el nom del producte que vol cercar l'usuari
	fgets(prod_name, MAX_STRING_LENGTH-1, stdin);
	// Elimino el '\n'
	prod_name[strlen(prod_name) - 1] = '\0';

	// Faig un bucle for per tal de cercar si hi ha algun producte guardat a p amb el mateix nom
	for(i = 0; i<prod_num;i++){
		if(strcmp(prod_name, p[i].name) == 0){

		// En cas de trobar un producte amb el mateix nom, mostro la informació a pantalla.
		printf("\nProduct found!\nName: %s\nStock: %d units\nPrice: %.2f euros\n", prod_name, p[i].stock, p[i].price);
		prod_found = 1;
		}
	}
	// Del contrari, mostro un missatge d'error
	if (!prod_found){
		printf("\nERROR: Product not found\n");
	}

}
/*********************************************** 
*
* @Finalitat: Printar una llista de tots els productes guardats ordenats segons el seu stock
* @Paràmetres: in: p = array del tipus Product on estàn guardats tots els Productes
* 			   in: prod_num = numero de productes que hi ha guardats a p[]
* @Retorn: ----
*
* **********************************************/
void listProd(Product p[], int prod_num){
	int i = 0, j = 0;					// Variables utilitzades per als bucles for
	int min = 1000;							// Variable que serà el stock minim en cada moment
	int min_pos = 0;						// Variable on es guardarà la posició en la que està el producte amb el stock minim
	Product p_aux;							// Variable de tipus Product on es guardarà el producte substituit de forma temporal 
	
	// Comprovo que hi hagi productes guardats a la tenda
	if(prod_num == 0){
		printf("\nThere are 0 products in the store\n");
	}
	// Del contrari ordeno els productes de menor a major stock
	else{
		// Faig un bucle for que recorrera les posicions on anar recolocant els valors
		for(j=0;j<prod_num;j++){
			min = 1000;
			// Faig un altre bucle for que anirà comprovant quin és el producte amb stock minim només de les posicions que no han estat mogudes
			for(i=j;i<prod_num;i++){
				if (p[i].stock < min){
					min = p[i].stock;
					min_pos = i;
				}
			}
			// Coloco el producte que hi ha a la posició j a p_aux
			p_aux = p[j];

			// A la posició j coloco el producte amb stock minim
			p[j] = p[min_pos];

			// A la posició on estava el stock minim, torno a colocar el producte que en un principi estava a la posició j
			p[min_pos] = p_aux;
		}
		printf("\n");

		// Faig un bucle for per mostrar tots els productes un cop ordenats
		for(i=0;i<prod_num;i++){
			printf("Product #%d:\nName: %s\nStock: %d units\nPrice: %.2f euros\n---\n", i+1, p[i].name, p[i].stock, p[i].price);
		}
	}
}
/*********************************************** 
*
* @Finalitat: Executar el sistema (Funció principal).
* @Paràmetres: ----
* @Retorn: ----
*
* **********************************************/
int main(){
	
		FILE *f;									// Variable del fitxer
		int op = 0;									// Variable on es guardarà la operació introduida per l'usuari
		int flag = 0, sup_flag = 0;					// Variables flag que seran utilitzades per als bucles while
		int sup_num = 0;							// Variable a la qual se li asignara el numero del supplier escollit
		char file_name[MAX_STRING_LENGTH];			// Nom del fitxer a obrir
		int valid = 0;								// Variable que indicarà si el fitxer és valid o no
		Product p[30];								// Array de tipus Product on es guardaran els productes valids
		int prod_num = 0;							// numero de productes que hi ha guardats a p[]
		

		printf("Welcome!\n");	

		// Faig un bucle while per tal de que la unica manera de sortir del sistema sigui amb el exit
		while(!flag){

				// Mostro les opcions disponibles i escanejo la que escull l'usuari
				printf("\n1. Load products from supplier | 2. List products | 3. Show product | 4. Exit\n");
				printf("Select option: ");
				scanf("%d", &op);

				// Comprovo que la opció sigui vàlida
				if (op < 1 || op > 4){
						printf("\nERROR: Wrong option number\n");
				}
				else{

					// Faig un switch amb les 4 opcions
					switch (op) {
						case 1:
								// Faig un bucle do while per repetir la opció si el numero que introdueix l'usuari és incorrecte
								do{
								sup_flag = 0;
								// Mostro les opcions disponibles i escanejo la que escull l'usuari
								printf("\n1. Supplier 1 | 2. Supplier 2 | 3. Supplier 3\n");
								printf("Choose one to load from: ");
								scanf("%d", &sup_num);

								// Comprovo que aquesta sigui correcte
								if(sup_num<4 && sup_num>0){
								
								// Afegeixo la extensió amb la funció findFileName
								findFileName(sup_num, file_name);
								
								// Faig un switch per llegir el fitxer en els 3 formats diferents
								switch (sup_num) {
									case 1:
									// Obro el fitxer en mode lectura
									f = fopen(file_name, "r");

									// Comprovo si el fitxer és valid amb la funció validFile
									valid = validFile(file_name, f);
							        			
									if (valid){
										// Crido la funció loadSupplier1 per carregar els productes i la igualo a prod_num per actualitzar el numero de productes guardats 
										prod_num = loadSupplier1(p, f, prod_num); 
										// Tanco el fitxer
										fclose(f);
									}
									break;
									case 2:
									// Obro el fitxer en mode lectura
									f = fopen(file_name, "r");

									// Comprovo si el fitxer és valid amb la funció validFile
									valid = validFile(file_name, f);
									
									if(valid){
										// Crido la funció loadSupplier2 per carregar els productes i la igualo a prod_num per actualitzar el numero de productes guardats
										prod_num = loadSupplier2(p, f, prod_num);
										// Tanco el fitxer
										fclose(f);
									}
									break;
									case 3:
									// Obro el fitxer en mode lectura binaria
									f = fopen(file_name, "rb");

									// Comprovo si el fitxer és valid amb la funció validFile
									valid = validFile(file_name, f);
									
									if(valid){
										// Crido la funció loadSupplier3 per carregar els productes i la igualo a prod_num per actualitzar el numero de productes guardats
										prod_num = loadSupplier3(p, f, prod_num);
										// Tanco el fitxer
										fclose(f);
									}
									break;
								}
								}
								else{
								// En cas de que el numero no sigui correcte, mostro el error per pantalla i faig que es repeteixi el bucle
								printf("\nERROR: Wrong option number\n");

								sup_flag = 1;
								}
								}while(sup_flag);

								break;
								
						case 2:
								// Crido la funcio listProd per mostrar la llista de productes
								listProd(p, prod_num);
								break;
						case 3: 
								// Crido la funció findProdName per buscar un nom d'un producte guardat i mostrar les seves característiques
								findProdName(p, prod_num);
								break;
						case 4:
								printf("\nBye!\n");
								// Flag = 1 per acabar el bucle
								flag = 1;
								break;
						}
				}
		}
		return(0);
}
