# 5. Arquitetura do Banco de Dados (FileDB)

Uma das características mais importantes do `DBSrv` é sua abordagem para a persistência de dados.

## Sistema Baseado em Arquivos

O `DBSrv` **não utiliza um sistema de gerenciamento de banco de dados tradicional** como MySQL, PostgreSQL ou SQL Server. Em vez disso, ele implementa um "banco de dados" baseado em arquivos, onde a classe `CFileDB` é a principal interface para essas operações.

### Como Funciona:

-   **Contas:** Cada conta de usuário corresponde a um arquivo binário em uma estrutura de diretórios (ex: `account/ab/admin.dat`). O nome do arquivo geralmente é o login do usuário.
-   **Personagens:** Os dados de todos os personagens de uma conta (geralmente até 4) são armazenados dentro do mesmo arquivo da conta. Isso inclui:
    -   Status (Força, Inteligência, etc.).
    -   Inventário.
    -   Itens equipados.
    -   Baú (warehouse).
-   **Operações:** Quando um servidor de jogo solicita "carregar um personagem", o `DBSrv` localiza o arquivo da conta, o lê para a memória, encontra os dados do personagem específico e os envia de volta. O processo de salvar segue o caminho inverso: recebe os dados do personagem e sobrescreve a seção correspondente no arquivo da conta.

### Vantagens e Desvantagens

-   **Vantagens:**
    -   **Simplicidade:** Nenhuma dependência externa de um serviço de banco de dados. O servidor é autocontido.
    -   **Portabilidade:** Para fazer backup, basta copiar os diretórios de contas.
-   **Desvantagens:**
    -   **Escalabilidade Limitada:** Acesso concorrente a arquivos é complexo e pode se tornar um gargalo.
    -   **Risco de Corrupção:** Uma falha de energia ou um bug durante a escrita de um arquivo pode corromper permanentemente os dados de uma conta.
    -   **Consultas Complexas:** É impossível realizar consultas complexas (ex: "encontrar todos os personagens com o item X") sem ler todos os arquivos de todas as contas.
