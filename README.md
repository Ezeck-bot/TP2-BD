###  Basic database project

Pour cloner le project:
`git clone --recursive https://github.com/chemin.vers.repos.git`

Si jamais vous ne voyez pas zig.exe dans Tools, FoundryTools_windowsx64, faite:
`.\setup.bat`

Pour compiler le project:
`ctrl+shift+b`

Pour debugger le project:
`F5`


```mermaid
    erDiagram
    Race ||--o{ Classe : has
    Factions ||--o{ Classe : has
    SocialStatus ||--o{ Classe : has
    Items ||--o{ Inventory : has
    Classe ||--o{ Character : has
    Character ||--o{ Customer : has
    Items ||--o{ Wallet : has
    Wallet ||--o{ Customer : has
    CreditCard ||--o{ Customer : has
    TransactionCreditCard ||--o{ CreditCard : has
    Commandes ||--o{ Customer : has
    Commandes ||--o{ Items : has

    Customer {
        int CustumerId
        string username
        string password
        int CharacterId FK
        int WalletId FK
        int CreditCardId FK
    }

    CreditCard{
        int CreditCardId
        string card_number
        string name_on_card
        string cvc_number
        int register
    }

    Wallet {
        int WalletId
        int ggbucks_amout
        int ItemsId FK
    }

    Race {
        int RaceId
        string name
        string description
    }

    Factions {
        int FactionId
        string name
        string description
    }

    SocialStatus {
        int FactionId
        string name
        string description
    }

    Classe {
        int ClasseId
        int RaceId FK
        int FactionId FK
        int FactionId FK
    }

    Items {
        int ItemsId
        text name
        int value
        int is_weapon
        int is_armor
        int is_equipped
    }

    Inventory {
        int InventoryId
        int ItemsId FK
    }

    Character {
        int CharacterId
        int level
        int strength
        int stamina
        int stealth
        int charisma
        int ClasseId FK
    }

    TransactionCreditCard {
        int transactionCreditCardId
        int ggbucks_amout
        date date
        int CreditCardId FK
    }

    Commandes {
        int commandeId
        int ggbucks_depense
        date date
        int CustumerId FK
        int ItemsId FK
    }
```