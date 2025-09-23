# 4. Processamento de Pacotes de Rede

Quando o `NetworkManager` detecta dados em um socket, ele invoca os callbacks `onUserData` ou `onAdminData`. Estes, por sua vez, chamam os métodos do `MessageHandler` para processar os pacotes.

## `MessageHandler::processClientMessage(sessionId, msg)`

Esta função trata as requisições vindas dos **servidores do jogo**.

-   **Validação:** Realiza uma verificação básica no cabeçalho do pacote para garantir que é uma mensagem válida do tipo `FLAG_GAME2DB`.
-   **Delegação:** A lógica de negócio é delegada para os módulos apropriados, principalmente o `DataManager` e o `DatabaseManager`. O `MessageHandler` atua como um roteador, interpretando a requisição (ex: "carregar personagem", "salvar item no baú") e chamando os métodos correspondentes nos outros módulos para executar a operação.

## `MessageHandler::processAdminMessage(sessionId, msg)`

Esta função trata os comandos vindos dos **clientes de administração**.

-   **Lógica de Comandos:** Contém uma grande estrutura `switch` que mapeia o tipo de mensagem para uma ação específica.
-   **Funcionalidades de Administração:**
    -   Autenticação do administrador (`_MSG_NPIDPASS`).
    -   Busca de dados de contas e personagens (`_MSG_NPReqAccount`).
    -   Modificação e salvamento de contas (`_MSG_NPReqSaveAccount`).
    -   Banimento e desbanimento de contas (`_MSG_NPDisable`, `_MSG_NPEnable`).
    -   Envio de avisos para todos os jogadores online (`_MSG_NPNotice`).
    -   Criação de personagens como parte de um sistema de transferência.