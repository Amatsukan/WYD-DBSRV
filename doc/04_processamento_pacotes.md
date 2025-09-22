# 4. Processamento de Pacotes de Rede

Depois que `MainWndProc` recebe um evento de leitura (`WSA_READ` ou `WSA_READADMIN`), os dados brutos são passados para funções específicas que entendem o protocolo da aplicação.

## `ProcessClientMessage(conn, msg)`

Esta função trata as requisições vindas dos **servidores do jogo**.

-   **Validação:** Realiza uma verificação básica no cabeçalho do pacote para garantir que é uma mensagem válida do tipo `FLAG_GAME2DB`.
-   **Delegação:** O trabalho pesado é delegado para o objeto `cFileDB` (através da chamada `cFileDB.ProcessMessage(msg, conn)`). A classe `CFileDB` contém a lógica para interpretar a requisição específica (ex: "carregar personagem", "salvar item no baú") e executar a operação correspondente no sistema de arquivos.

## `ProcessAdminMessage(conn, msg)`

Esta função trata os comandos vindos dos **clientes de administração**.

-   **Lógica de Comandos:** Contém uma grande estrutura `switch` que mapeia o tipo de mensagem para uma ação específica.
-   **Funcionalidades de Administração:**
    -   Autenticação do administrador (`_MSG_NPIDPASS`).
    -   Busca de dados de contas e personagens (`_MSG_NPReqAccount`).
    -   Modificação e salvamento de contas (`_MSG_NPReqSaveAccount`).
    -   Banimento e desbanimento de contas (`_MSG_NPDisable`, `_MSG_NPEnable`).
    -   Envio de avisos para todos os jogadores online (`_MSG_NPNotice`).
    -   Criação de personagens como parte de um sistema de transferência.
