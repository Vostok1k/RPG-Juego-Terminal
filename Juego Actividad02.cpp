#include <iostream>
#include <windows.h> // libreria para reproducir sonido
#include <mmsystem.h>
#include <string>
#include <time.h>

using namespace std;

/* CHARACTER STATS */
int characterHP = 110;
int characterDamage = 0;
int meleeDamage = 10; /* maximo de vida que pueden quitar los atques */
int swordDamage = 20;
int magicDamage = 30;
int magicUses = 3; /* limite de 3 usos de magia */
string characterName;
bool heroIsAlive;

/* BOSS 1 STATS */
int bossHP = 80;
int realBossDamage = 15;
int randomBossDamage;
string bossName = "Odium";

/* BOSS 2 STATS */
int boss2HP = 50;
int realBoss2Damage = 20;
int randomBoss2Damage;
string boss2Name = "Ruina";

int bossId; /* escoger boss */
int idDamage; /* escoger ataque */
bool idDamageOk = false; /* verificar que el ataque escogido es correcto */

void IntroducirNombre() {
	cout << "Escribe el nombre de tu heroe: "; /* nombre de personaje */
	cin >> characterName;
	PlaySound(TEXT("start.wav"), NULL, SND_FILENAME);
}


void EscogerAtaque() {
	cout << "\nCon que quieres atacar?\n1 - Golpe\n2 - Espada\n3 - Magia (" << magicUses << ")\n\nTipo de ataque: ";
	cin >> idDamage;
	cout << "\n";
	switch (idDamage) {
	case 1:
		characterDamage = meleeDamage; /* se iguala al daño que has escogido */
		idDamageOk = true; /* para poder salir del bucle */
		PlaySound(TEXT("melee.wav"), NULL, SND_FILENAME);
		break;
	case 2:
		characterDamage = swordDamage;
		idDamageOk = true;
		PlaySound(TEXT("sword.wav"), NULL, SND_FILENAME);
		break;
	case 3:
		if (magicUses > 0) {  /* entra si te queda magia */
			characterDamage = magicDamage;
			idDamageOk = true;
			magicUses--;
			PlaySound(TEXT("magic.wav"), NULL, SND_FILENAME);
		}
		else { /* se ha superado el uso maximo de la magia */
			cout << "-------------------WARNING---------------------\n";
			cout << "Ya no te queda magia, prueba con otro atque.\n";
			cout << "-----------------------------------------------\n";
			PlaySound(TEXT("limit.wav"), NULL, SND_FILENAME);
		}
		break;

	default:
		cout << "-------------------ERROR-----------------------\n";
		cout << "Tipo de ataque incorrecto, vuelve a intentarlo.\n";
		cout << "-----------------------------------------------\n";
		PlaySound(TEXT("limit.wav"), NULL, SND_FILENAME);
		break;
	}
}


int EscogerEnemigo() {
	int enemigo;
	cout << "\nA quien quieres atacar?\n1 - Odium\n2 - Ruina\n\nEnemigo: "; /* escoger enemigo */
	cin >> enemigo;
	PlaySound(TEXT("select.wav"), NULL, SND_FILENAME);
	return enemigo;
}



bool RealizarAtaque() {

	switch (bossId) { /* segun el boss que hayas escogido se ejecuta una peleo o otra */
	case 1: /* PELEA BOSS 1 */
		randomBossDamage = rand() % realBossDamage + 1;
		characterHP = characterHP - randomBossDamage; /* el enemigo pega daño aleatorio */

		if (characterHP < 0) {
			characterHP = 0; /* si la vida de character es menor a 0 (ej. -1) se iguala a 0 */
		}

		if (characterHP == 0) { /* si la vida es igual a 0 sale mensaje de que has muerto */
			return false;
		}
		break;

	case 2: /* PELEA BOSS 2 */
		randomBoss2Damage = rand() % realBoss2Damage + 1;
		characterHP = characterHP - randomBoss2Damage;

		if (characterHP < 0) {
			characterHP = 0;
		}

		if (characterHP == 0) {
			return false;
		}
		break;
	}
}

int main()
{	
	srand(time(NULL));
	IntroducirNombre();
	while (characterHP != 0 && (bossHP != 0 || boss2HP != 0)) {
		bossId = EscogerEnemigo();
		if ((bossId == 1 && bossHP == 0) || (bossId == 2 && boss2HP == 0)) { /* si el enemigo escogido esta muerto sale error */
			cout << "\n-------------------ERROR-----------------------\n";
			cout << "Este enemigo ya esta muerto, escoge otro.\n";
			cout << "-----------------------------------------------\n";
			PlaySound(TEXT("limit.wav"), NULL, SND_FILENAME);
		}
		else if ((bossId < 1) || (bossId > 2)) { /* Avisa si el id de boss escogido es incorrecto */
			cout << "\n-------------------ERROR-----------------------\n";
			cout << "Enemigo incorrecto, intentalo de nuevo.\n";
			cout << "-----------------------------------------------\n";
			PlaySound(TEXT("limit.wav"), NULL, SND_FILENAME);
		}
		else { /* Si las condiciones anteriores no se ejecutan puedes continuar */
			while (idDamageOk == false) { /* interruptor para escoger ataque, no puedes continuar hasta que escojas el ataque correcto "idDamageOk == true" */				
				cout << "\n";
				EscogerAtaque();
			}
			heroIsAlive = RealizarAtaque();
			if (heroIsAlive == false) {
				switch (bossId) {
				case 1:
					cout << "                      ****** " << bossName << " te ha matado. " << "Game Over ******\n\n";
					PlaySound(TEXT("die.wav"), NULL, SND_FILENAME);
					break;
				case 2:
					cout << "                      ****** " << boss2Name << " te ha matado. " << "Game Over ******\n\n";
					PlaySound(TEXT("die.wav"), NULL, SND_FILENAME);
					break;
				}
			} else {
				switch (bossId) {
				case 1:
					cout << "------------------------------------- BATALLA ----------------------------------------\n";
					cout << bossName << " te ha pegado " << randomBossDamage << " de vida, te queda " << characterHP << " de vida.\n";

					characterDamage = rand() % characterDamage + 1;
					bossHP = bossHP - characterDamage;

					if (bossHP < 0) {
						bossHP = 0;
					}

					cout << characterName << " le ha pegado un tortazo de " << characterDamage << " de vida y a " << bossName << " le quedan " << bossHP << " de vida\n\n";
					cout << "--------------------------------------------------------------------------------------\n\n";

					if (bossHP == 0) {
						cout << "                      ****** Has derrotado a " << bossName << " ******\n\n";
						PlaySound(TEXT("kill.wav"), NULL, SND_FILENAME);
					}
					break;

				case 2:
					cout << "------------------------------------- BATALLA ----------------------------------------\n";
					cout << boss2Name << " te ha pegado " << randomBoss2Damage << " de vida, te queda " << characterHP << " de vida.\n";

					characterDamage = rand() % characterDamage + 1;
					boss2HP = boss2HP - characterDamage;

					if (boss2HP < 0) {
						boss2HP = 0;
					}

					cout << characterName << " le ha pegado un tortazo de " << characterDamage << " de vida y a " << boss2Name << " le quedan " << boss2HP << " de vida\n\n";
					cout << "--------------------------------------------------------------------------------------\n\n";

					if (boss2HP == 0) {
						cout << "                      ****** Has derrotado a " << boss2Name << " ******\n\n";
						PlaySound(TEXT("kill.wav"), NULL, SND_FILENAME);
					}
					break;
				}
			}
		}
		idDamageOk = false; /* para volver a entrar al bucle de escoger ataque la variable idDamageOk tiene que ser false, si has llegado hasta
							agui significa que es true y no volveria a preguntar cual ataque quieres hacer */
	}
	if ((bossHP == 0) && (boss2HP == 0)) {
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		cout << "                                                *****************************\n";
		cout << "                                         ***************** YOU WIN *****************\n";
		cout << "                                                *****************************";
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		PlaySound(TEXT("win.wav"), NULL, SND_FILENAME);
	}
	return 0;
}
