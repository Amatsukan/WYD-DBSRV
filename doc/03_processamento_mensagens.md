# 3. Processamento de Eventos e Comandos

Com a arquitetura baseada em console, o processamento de eventos é dividido entre a entrada de comandos do administrador e o tratamento de eventos de rede de forma assíncrona.

## 1. Comandos do Console

O servidor inicia uma thread dedicada (`handleConsoleInput`) para ler a entrada do console de forma contínua, permitindo que um administrador gerencie o servidor em tempo real.

-   **Comandos Disponíveis:**
    -   `exit`: Inicia o processo de desligamento seguro do servidor.
    -   `status`: Exibe o status atual das sessões de usuários (servidores de jogo) e administradores conectados.

## 2. Eventos de Rede (Callbacks)

O `NetworkManager` gerencia todas as conexões de baixo nível e notifica o `Server` sobre eventos de rede através de callbacks. Isso elimina a necessidade de um loop de mensagens do Windows.

-   **`onUserConnect(sessionId)`:**
    -   Chamado quando um novo servidor de jogo se conecta.
    -   O `Server` registra a conexão e notifica o `MessageHandler`.

-   **`onUserDisconnect(sessionId)`:**
    -   Chamado quando um servidor de jogo se desconecta.
    -   O `Server` limpa a sessão.

-   **`onUserData(sessionId)`:**
    -   Chamado quando um pacote de dados é recebido de um servidor de jogo.
    -   O `Server` delega o processamento dos dados para o `MessageHandler`, que irá decodificar e tratar a mensagem específica (ex: login de conta, salvar personagem).

-   **Callbacks de Administração (`onAdminConnect`, `onAdminDisconnect`, `onAdminData`):**
    -   Funcionam de maneira análoga aos callbacks de usuário, mas para o canal de administração.
    -   No `onAdminConnect`, há uma verificação de segurança para garantir que o IP do administrador está na lista de IPs autorizados (`Admin.txt`).

## 3. Tarefas Periódicas (`TaskManager`)

O `TaskManager` executa tarefas agendadas em uma thread separada, como:
-   Atualizações periódicas de dados (`periodicUpdate` no `DataManager`).
-   Importação de itens, doações e usuários.
-   Atualização de senhas.
-   Geração de logs diários.
-   Reset semanal de rankings.

Este modelo assíncrono é mais eficiente e adequado para uma aplicação de servidor, que precisa lidar com múltiplas operações simultâneas sem depender de uma interface gráfica.