#include "database.h"
#include "sqlite3.h"
#include "knob.h"
#include "md5.h"
#include <time.h>

#define LOG_SQLITE3_ERROR(db) knob_log(KNOB_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))

//--ADD TABLE
const char* create_table[] = {
    "CREATE TABLE IF NOT EXISTS Items(itemsId INTEGER PRIMARY KEY NOT NULL,name TEXT NOT NULL,value INT NOT NULL, is_weapon INT NOT NULL, is_amor INT NOT NULL, is_equiped INT NOT NULL);",
    "CREATE TABLE IF NOT EXISTS Inventory(inventotyId INTEGER PRIMARY KEY NOT NULL, itemsId INTEGER, FOREIGN KEY (itemsId) REFERENCES Items(itemsId));",
    "CREATE TABLE IF NOT EXISTS Classe(classeId INTEGER PRIMARY KEY NOT NULL,race INT NOT NULL,faction INT NOT NULL,socialstatus INT NOT NULL);",
    "CREATE TABLE IF NOT EXISTS Character(characterId INTEGER PRIMARY KEY NOT NULL,name TEXT NOT NULL, level INT NOT NULL,strength INT NOT NULL,stamina INT NOT NULL,stealth INT NOT NULL,charisma INT NOT NULL, classeId INTEGER, FOREIGN KEY (classeId) REFERENCES Classe(ClasseId));",
    "CREATE TABLE IF NOT EXISTS CreditCard(creditCardId INTEGER PRIMARY KEY NOT NULL,card_number TEXT NOT NULL,card_expiration TEXT NOT NULL,cvc_number TEXT NOT NULL, register INT);",
    "CREATE TABLE IF NOT EXISTS TranscationCreditCard(transactionCreditCardId INTEGER PRIMARY KEY NOT NULL,ggbucks_amout INT,date TEXT NOT NULL, creditCardId INTEGER, FOREIGN KEY (creditCardId) REFERENCES CreditCard(creditCardId));",
    "CREATE TABLE IF NOT EXISTS Client(ClientId INTEGER PRIMARY KEY NOT NULL,username TEXT NOT NULL,password TEXT NOT NULL, characterId INTEGER, walletId INTEGER, CreditCardId INTEGER, FOREIGN KEY (characterId) REFERENCES Character(characterId), FOREIGN KEY (walletId) REFERENCES Wallet(walletId), FOREIGN KEY (creditCardId) REFERENCES CreditCard(creditCardId));",
    "CREATE TABLE IF NOT EXISTS Wallet(walletId INTEGER PRIMARY KEY NOT NULL, ggbucks_amout INT ,ClientId INTEGER, FOREIGN KEY (ClientId) REFERENCES Client(ClientId));",
    "CREATE TABLE IF NOT EXISTS Commandes(commandeId INTEGER PRIMARY KEY NOT NULL, ClientId INTEGER, itemsId INTEGER, ggbucks_depense INT, date TEXT, FOREIGN KEY (itemsId) REFERENCES Items(itemsId), FOREIGN KEY (ClientId) REFERENCES Client(ClientId));"
};
int size_create_table = sizeof(create_table) / sizeof(create_table[0]);

void database_start(GameData* g){
    sqlite3_open("game.db", &g->db);
}
void database_init_tables(GameData* g){

    sqlite3_stmt *stmt_db = NULL;

    for (int i = 0; i < size_create_table; i++)
    {
        int ret_db = sqlite3_prepare_v2(g->db, create_table[i], -1, &stmt_db, NULL);

        if (ret_db != SQLITE_OK)
        {
            LOG_SQLITE3_ERROR(g->db);
        }
    
        ret_db = sqlite3_step(stmt_db);
    }

    sqlite3_finalize(stmt_db);

}
void database_insert_tables(GameData* g){
    //CLASSE
    //Nord = 0; Dark_Elf = 1; Redguard = 2; Orc = 3;
    //Dawnguard = 0; Forsworm = 1; BrotherhoodOfSteel = 2; Imperials = 3
    //Citizen = 0; Merchant = 1; Soldier = 2;
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt = NULL;

    char content[] = "INSERT INTO Classe(race, faction, socialstatus) VALUES (0, 0, 2), (1, 3, 1), (2, 2, 2), (3, 1, 2),(0, 3, 0);";

    int ret = sqlite3_prepare_v2(g->db, content, -1, &stmt, NULL);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    
    sqlite3_finalize(stmt);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //items
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_item = NULL;

    char content_item[] = "INSERT INTO Items(name, value, is_weapon, is_amor, is_equiped) VALUES ('Sword', 20, 1, 0, 0), ('Amor', 25, 0, 1, 0), ('Shield', 20, 0, 1, 0), ('Bow', 40, 1, 0, 0),('Helmet', 15, 0, 1, 0), ('hatchet', 20, 1, 0, 0);";

    int ret_item = sqlite3_prepare_v2(g->db, content_item, -1, &stmt_item, NULL);

    if (sqlite3_step(stmt_item) != SQLITE_DONE)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    
    sqlite3_finalize(stmt_item);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
}
void database_end(GameData* g){
    sqlite3_close(g->db);
}

void database_create_user(GameData* g, char* username, char* mdp){

    //hache mot de passe
    // uint8_t mdp_hache[256];
    // md5String(mdp, mdp_hache);

    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt3 = NULL;

    char content3[] =
        "INSERT INTO Client(username, password, characterId, walletId, CreditCardId)\n"
        "VALUES\n"
        "(?,?,?,?,?);";
    
    int ret3 = sqlite3_prepare_v2(g->db, content3, -1, &stmt3, NULL);

    if (sqlite3_bind_text(stmt3, 1, username, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    if (sqlite3_bind_text(stmt3, 2, mdp, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt3,3,0) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt3,4,0) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt3,5,0) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if (sqlite3_step(stmt3) != SQLITE_DONE)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    
    sqlite3_finalize(stmt3);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //RECUP LAST ID Client
    sqlite3_stmt* stmt_last_client_id = NULL;
    char content_last_client_id[] = "SELECT ClientId FROM Client ORDER BY ClientId DESC;";
    int ret_last_client_id = sqlite3_prepare_v2(g->db,content_last_client_id,-1,&stmt_last_client_id,NULL);

    if(ret_last_client_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_last_client_id = sqlite3_step(stmt_last_client_id);
        
    if(ret_last_client_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
    }

    int column_last_client_id = 0;
    int ClientId = sqlite3_column_int(stmt_last_client_id,column_last_client_id);
    sqlite3_finalize(stmt_last_client_id);

    //CREER SON WALLET
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_wallet = NULL;

    char content_wallet[] =
        "INSERT INTO Wallet(ggbucks_amout, ClientId)\n"
        "VALUES\n"
        "(?,?);";
    
    int ret_wallet = sqlite3_prepare_v2(g->db, content_wallet, -1, &stmt_wallet, NULL);

    if(sqlite3_bind_int(stmt_wallet,1,0) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt_wallet,2,ClientId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if (sqlite3_step(stmt_wallet) != SQLITE_DONE)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    
    sqlite3_finalize(stmt_wallet);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //RECUP LAST ID WALLET
    sqlite3_stmt* stmt_wallet_id = NULL;
    char content_wallet_id[] = "SELECT walletId FROM Wallet ORDER BY walletId DESC;";
    int ret_wallet_id = sqlite3_prepare_v2(g->db,content_wallet_id,-1,&stmt_wallet_id,NULL);

    if(ret_wallet_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_wallet_id = sqlite3_step(stmt_wallet_id);
        
    if(ret_wallet_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
    }

    int column_wallet_id = 0;
    int creditCardId = sqlite3_column_int(stmt_wallet_id,column_wallet_id);
    sqlite3_finalize(stmt_wallet_id);

    //UPDATE CLIENT
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_update_client = NULL;

    char content_update_client[] =
        "UPDATE Client SET walletId = ?\n"
        "WHERE ClientId = ?;";

    int ret_update_client = sqlite3_prepare_v2(g->db, content_update_client, -1, &stmt_update_client, NULL);

    if(sqlite3_bind_int(stmt_update_client,1,creditCardId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt_update_client,2,ClientId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_update_client = sqlite3_step(stmt_update_client);
    
    sqlite3_finalize(stmt_update_client);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
}

int database_login_user(GameData* g, char* username, char* mdp){

    //hache mot de passe
    // uint8_t mdp_hache[256];
    // md5String(mdp, mdp_hache);

    // knob_log(KNOB_INFO, mdp_hache);

    sqlite3_stmt* stmt_verif_password = NULL;
    char content_verif_password[] =
    "SELECT password FROM Client WHERE username = ?;";

    int ret_verif_password = sqlite3_prepare_v2(g->db,content_verif_password,-1,&stmt_verif_password,NULL);

    if(ret_verif_password != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if (sqlite3_bind_text(stmt_verif_password, 1, username, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    char* password = NULL;
    for(int ret_verif_password = sqlite3_step(stmt_verif_password);ret_verif_password != SQLITE_DONE;ret_verif_password = sqlite3_step(stmt_verif_password)){
        if(ret_verif_password != SQLITE_ROW){
            LOG_SQLITE3_ERROR(g->db);
        }
        int column = 0;
        const unsigned char* raw_password = sqlite3_column_text(stmt_verif_password, column++);
        password = knob_temp_strdup((const char*)raw_password);
    }
    sqlite3_finalize(stmt_verif_password);
    
    if (knob_cstr_match(password, mdp))
    {
        return 1;
    } 
    return 0;
}

int verification_class_exists(GameData* g, int race_active, int faction_active, int status_active){
    //Cherchons la classe
    sqlite3_stmt* stmt_classe = NULL;
    char content[] = "SELECT classeId FROM Classe WHERE race = ? AND faction = ? AND socialstatus = ?;";
    int ret_classe = sqlite3_prepare_v2(g->db,content,-1,&stmt_classe,NULL);

    if(ret_classe != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt_classe,1,race_active) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt_classe,2,faction_active) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt_classe,3,status_active) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_classe = sqlite3_step(stmt_classe);
        
    if(ret_classe != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
    }

    int column = 0;
    int classeId = sqlite3_column_int(stmt_classe,column);
    sqlite3_finalize(stmt_classe);
    
    return classeId;
}

void database_save_character(GameData* g, char* username, int classId, int level, int strength, int stamina, int stealth, int charisma){

    //--------------

    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt3 = NULL;

    char content3[] =
        "INSERT INTO Character(name, level, strength, stamina, stealth, charisma, classeId)\n"
        "VALUES\n"
        "(?,?,?,?,?,?,?);";
    
    int ret3 = sqlite3_prepare_v2(g->db, content3, -1, &stmt3, NULL);

    if (sqlite3_bind_text(stmt3, 1, username, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt3,2,level) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt3,3,strength) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt3,4,stamina) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt3,5,stealth) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt3,6,charisma) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt3,7,classId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if (sqlite3_step(stmt3) != SQLITE_DONE)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    
    sqlite3_finalize(stmt3);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //RECUP LAST ID CHARACTER ENREGISTRE
    sqlite3_stmt* stmt_character_id = NULL;
    char content[] = "SELECT characterId FROM Character ORDER BY characterId DESC;";
    int ret_character_id = sqlite3_prepare_v2(g->db,content,-1,&stmt_character_id,NULL);

    if(ret_character_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_character_id = sqlite3_step(stmt_character_id);
        
    if(ret_character_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
    }

    int column = 0;
    int characterId = sqlite3_column_int(stmt_character_id,column);
    sqlite3_finalize(stmt_character_id);

    //UPDATE CLIENT
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_update_client = NULL;

    char content_update_client[] =
        "UPDATE Client SET characterId = ?\n"
        "WHERE ClientId = 1;";

    int ret_update_client = sqlite3_prepare_v2(g->db, content_update_client, -1, &stmt_update_client, NULL);

    if(sqlite3_bind_int(stmt_update_client,1,characterId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_update_client = sqlite3_step(stmt_update_client);
    
    sqlite3_finalize(stmt_update_client);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
}

int database_verification_insert_table(GameData* g){
    sqlite3_stmt* stmt_character_id = NULL;
    char content[] = "SELECT count(*) AS nombre FROM Classe;";
    int ret_character_id = sqlite3_prepare_v2(g->db,content,-1,&stmt_character_id,NULL);

    if(ret_character_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_character_id = sqlite3_step(stmt_character_id);
        
    if(ret_character_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
        return -1;
    }

    int column = 0;
    int nombre = sqlite3_column_int(stmt_character_id,column);
    sqlite3_finalize(stmt_character_id);

    return nombre;
}

int database_charactere_race_nombre(GameData* g){
    sqlite3_stmt* stmt_character_id = NULL;
    char content[] = "SELECT count(*) AS nombre FROM Classe,Character WHERE Character.classeId=Classe.classeId;";
    int ret_character_id = sqlite3_prepare_v2(g->db,content,-1,&stmt_character_id,NULL);

    if(ret_character_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_character_id = sqlite3_step(stmt_character_id);
        
    if(ret_character_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
        return -1;
    }

    int column = 0;
    int nombre = sqlite3_column_int(stmt_character_id,column);
    sqlite3_finalize(stmt_character_id);

    return nombre;
}

int database_charactere_race(GameData* g){
    sqlite3_stmt* stmt_character_id = NULL;
    char content[] = "SELECT Classe.race AS race FROM Classe,Character WHERE Character.classeId=Classe.classeId;";
    int ret_character_id = sqlite3_prepare_v2(g->db,content,-1,&stmt_character_id,NULL);

    if(ret_character_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_character_id = sqlite3_step(stmt_character_id);
        
    if(ret_character_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
        return -1;
    }

    int column = 0;
    int race = sqlite3_column_int(stmt_character_id,column);
    sqlite3_finalize(stmt_character_id);

    return race;
}

int database_see_ggbucks(GameData* g){
    sqlite3_stmt* stmt_ggbuncks = NULL;
    char content[] = "SELECT Wallet.ggbucks_amout AS ggbucks_amout FROM Wallet,Client WHERE Wallet.walletId=Client.walletId;";
    int ret_ggbuncks = sqlite3_prepare_v2(g->db,content,-1,&stmt_ggbuncks,NULL);

    if(ret_ggbuncks != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_ggbuncks = sqlite3_step(stmt_ggbuncks);
        
    if(ret_ggbuncks != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
        return -1;
    }

    int column = 0;
    int ggbucks_amout = sqlite3_column_int(stmt_ggbuncks,column);
    sqlite3_finalize(stmt_ggbuncks);

    return ggbucks_amout;
}

void database_add_credit_card(GameData* g, int card_number, char* card_expiration, int card_cvc, int number_ggbucks, int is_register_card){
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt3 = NULL;

    char content3[] =
        "INSERT INTO CreditCard(card_number, card_expiration, cvc_number, register)\n"
        "VALUES\n"
        "(?,?,?,?);";
    
    int ret3 = sqlite3_prepare_v2(g->db, content3, -1, &stmt3, NULL);

    if(sqlite3_bind_int(stmt3, 1, card_number) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if (sqlite3_bind_text(stmt3, 2, card_expiration, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt3, 3, card_cvc) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt3, 4, is_register_card) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if (sqlite3_step(stmt3) != SQLITE_DONE)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    
    sqlite3_finalize(stmt3);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
    //--

    //RECUP LAST ID CreditCard
    sqlite3_stmt* stmt_credit_card_id = NULL;
    char content[] = "SELECT creditCardId FROM CreditCard ORDER BY creditCardId DESC;";
    int ret_credit_card_id = sqlite3_prepare_v2(g->db,content,-1,&stmt_credit_card_id,NULL);

    if(ret_credit_card_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_credit_card_id = sqlite3_step(stmt_credit_card_id);
        
    if(ret_credit_card_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
    }

    int column = 0;
    int creditCardId = sqlite3_column_int(stmt_credit_card_id,column);
    sqlite3_finalize(stmt_credit_card_id);

    //--
    //ENREGISTRER DANS TRANSACTION
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_transac = NULL;

    char content_transac[] =
        "INSERT INTO TranscationCreditCard(ggbucks_amout, date, creditCardId)\n"
        "VALUES\n"
        "(?,?,?);";
    
    int ret = sqlite3_prepare_v2(g->db, content_transac, -1, &stmt_transac, NULL);

    if(sqlite3_bind_int(stmt_transac, 1, number_ggbucks) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if (sqlite3_bind_text(stmt_transac, 2, "2025-05-14", -1, SQLITE_STATIC) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt_transac, 3, creditCardId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret = sqlite3_step(stmt_transac);
    
    sqlite3_finalize(stmt_transac);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //UPDATE WALLET
    int last_ggbucks = database_see_ggbucks(g);
    int new_ggbucks = last_ggbucks + number_ggbucks;
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_update_wallet = NULL;

    char content_update_wallet[] =
        "UPDATE Wallet SET ggbucks_amout = ?\n"
        "WHERE walletId = 1;";

    int ret_update_wallet = sqlite3_prepare_v2(g->db, content_update_wallet, -1, &stmt_update_wallet, NULL);

    if(sqlite3_bind_int(stmt_update_wallet, 1, new_ggbucks) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_update_wallet = sqlite3_step(stmt_update_wallet);
    
    sqlite3_finalize(stmt_update_wallet);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //UPDATE CLIENT
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_update_client2 = NULL;

    char content_update_client2[] =
        "UPDATE Client SET CreditCardId = ?\n"
        "WHERE ClientId = 1;";

    int ret_update_client2 = sqlite3_prepare_v2(g->db, content_update_client2, -1, &stmt_update_client2, NULL);

    if(sqlite3_bind_int(stmt_update_client2,1,creditCardId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_update_client2 = sqlite3_step(stmt_update_client2);
    
    sqlite3_finalize(stmt_update_client2);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
}

Item* data_see_items(GameData* g, int* count){
    sqlite3_stmt* stmt_items = NULL;
    char content[] = "SELECT * FROM Items WHERE is_equiped = 0;";
    int ret_items = sqlite3_prepare_v2(g->db,content,-1,&stmt_items,NULL);

    if(ret_items != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    static Item items[256];
    *count = 0;

    for(int ret_items = sqlite3_step(stmt_items);ret_items != SQLITE_DONE;ret_items = sqlite3_step(stmt_items)){
        if(ret_items != SQLITE_ROW){
            LOG_SQLITE3_ERROR(g->db);
        }

        int column = 0;
        items[*count].id_inventaire = sqlite3_column_int(stmt_items, column++);
        const unsigned char* name = sqlite3_column_text(stmt_items, column++);
        strncpy(items[*count].name, (const char*)name, sizeof(items[*count].name) - 1);
        items[*count].name[sizeof(items[*count].name) - 1] = '\0';
        items[*count].value = sqlite3_column_int(stmt_items, column++);
        (*count)++;
    }

    sqlite3_finalize(stmt_items);

    return items;
}

void database_add_inventory(GameData* g, int id_items, int value_items){
    //marqué comme équiped dans Items
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_update_items = NULL;

    char content_update_items[] =
        "UPDATE Items SET is_equiped = 1\n"
        "WHERE itemsId = ?;";

    int ret_update_items = sqlite3_prepare_v2(g->db, content_update_items, -1, &stmt_update_items, NULL);

    if(sqlite3_bind_int(stmt_update_items, 1, id_items) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_update_items = sqlite3_step(stmt_update_items);
    
    sqlite3_finalize(stmt_update_items);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //RECUP LAST ID Client
    sqlite3_stmt* stmt_last_client_id = NULL;
    char content[] = "SELECT ClientId FROM Client ORDER BY ClientId DESC;";
    int ret_last_client_id = sqlite3_prepare_v2(g->db,content,-1,&stmt_last_client_id,NULL);

    if(ret_last_client_id != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_last_client_id = sqlite3_step(stmt_last_client_id);
        
    if(ret_last_client_id != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
    }

    int column = 0;
    int ClientId = sqlite3_column_int(stmt_last_client_id,column);
    sqlite3_finalize(stmt_last_client_id);

    //réduction des ggbucks dans  Wallet
    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_update_wallet = NULL;

    char content_update_wallet[] =
        "UPDATE Wallet SET ggbucks_amout = ggbucks_amout - ?\n"
        "WHERE ClientId = ?;";

    int ret_update_wallet = sqlite3_prepare_v2(g->db, content_update_wallet, -1, &stmt_update_wallet, NULL);

    if(sqlite3_bind_int(stmt_update_wallet, 1, value_items) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    if(sqlite3_bind_int(stmt_update_wallet, 2, ClientId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_update_wallet = sqlite3_step(stmt_update_wallet);
    
    sqlite3_finalize(stmt_update_wallet);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //ajouté dans commandes
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_commande = NULL;

    char content_commande[] =
        "INSERT INTO Commandes(ClientId, itemsId, ggbucks_depense, date)\n"
        "VALUES\n"
        "(?,?,?,?);";
    
    int ret_commande = sqlite3_prepare_v2(g->db, content_commande, -1, &stmt_commande, NULL);

    if(sqlite3_bind_int(stmt_commande, 1, ClientId) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt_commande, 2, id_items) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if(sqlite3_bind_int(stmt_commande, 3, value_items) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }
    if (sqlite3_bind_text(stmt_commande, 4, "2025-05-14", -1, SQLITE_STATIC) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_commande = sqlite3_step(stmt_commande);
    
    sqlite3_finalize(stmt_commande);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }

    //ajouté dans inventaire

    if (sqlite3_exec(g->db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
        return;
    }
    sqlite3_stmt *stmt_inventory = NULL;

    char content_inventory[] =
        "INSERT INTO Inventory(itemsId)\n"
        "VALUES\n"
        "(?);";
    
    int ret_inventory = sqlite3_prepare_v2(g->db, content_inventory, -1, &stmt_inventory, NULL);

    if(sqlite3_bind_int(stmt_inventory, 1, id_items) != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_inventory = sqlite3_step(stmt_inventory);
    
    sqlite3_finalize(stmt_inventory);

    if (sqlite3_exec(g->db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
    {
        LOG_SQLITE3_ERROR(g->db);
    }
}

int database_inventory_nombre(GameData* g){
    sqlite3_stmt* stmt_inventory_nb = NULL;
    char content[] = "SELECT count(*) AS nombre FROM Inventory,Items WHERE Inventory.itemsId=Items.itemsId";
    int ret_inventory_nb = sqlite3_prepare_v2(g->db,content,-1,&stmt_inventory_nb,NULL);

    if(ret_inventory_nb != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    ret_inventory_nb = sqlite3_step(stmt_inventory_nb);
        
    if(ret_inventory_nb != SQLITE_ROW){
        LOG_SQLITE3_ERROR(g->db);
        return -1;
    }

    int column = 0;
    int nombre = sqlite3_column_int(stmt_inventory_nb,column);
    sqlite3_finalize(stmt_inventory_nb);

    return nombre;
}

Inventaire* database_show_inventory(GameData* g, int* count){
    sqlite3_stmt* stmt_iventaires = NULL;
    char content_iventaires[] = "SELECT Items.itemsId ,Items.name AS name, Items.value AS value FROM Inventory,Items WHERE Inventory.itemsId=Items.itemsId;";
    int ret_iventaires = sqlite3_prepare_v2(g->db,content_iventaires,-1,&stmt_iventaires,NULL);

    if(ret_iventaires != SQLITE_OK){
        LOG_SQLITE3_ERROR(g->db);
    }

    static Inventaire iventaires[256];
    *count = 0;

    for(int ret_iventaires = sqlite3_step(stmt_iventaires);ret_iventaires != SQLITE_DONE;ret_iventaires = sqlite3_step(stmt_iventaires)){
        if(ret_iventaires != SQLITE_ROW){
            LOG_SQLITE3_ERROR(g->db);
        }

        int column = 0;
        iventaires[*count].id_inventaire = sqlite3_column_int(stmt_iventaires, column++);
        const unsigned char* name = sqlite3_column_text(stmt_iventaires, column++);
        strncpy(iventaires[*count].name, (const char*)name, sizeof(iventaires[*count].name) - 1);
        iventaires[*count].name[sizeof(iventaires[*count].name) - 1] = '\0';
        iventaires[*count].value = sqlite3_column_int(stmt_iventaires, column++);
        (*count)++;
    }

    sqlite3_finalize(stmt_iventaires);

    return iventaires;
}