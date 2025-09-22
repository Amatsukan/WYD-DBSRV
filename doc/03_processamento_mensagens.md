# 3. Processador de Eventos (`MainWndProc`)

A função `MainWndProc` é o coração do servidor, responsável por tratar todos os eventos recebidos pela janela principal. Os eventos mais importantes são os de rede, que são enviados pelo Windows como mensagens `WM_USER` customizadas.

## Principais Mensagens de Rede Tratadas

-   **`WSA_ACCEPT` (Nova Conexão de Servidor do Jogo):**
    -   Disparada quando um servidor do jogo tenta se conectar na `DB_PORT`.
    -   O `DBSrv` aceita a conexão e valida o IP do servidor.
    -   Se o IP for autorizado (presente na `ServerList.txt`), a conexão é mantida e o `DBSrv` envia os dados iniciais necessários (informações de guilds, guerras, etc.).

-   **`WSA_ACCEPTADMIN` (Nova Conexão de Administrador):**
    -   Ocorre quando um cliente de admin tenta se conectar na `ADMIN_PORT`.
    -   O IP é validado contra a lista de `Admin.txt`.
    -   Se autorizado, o `DBSrv` inicia o processo de autenticação (login/senha).

-   **`WSA_READ` (Dados Recebidos de um Servidor do Jogo):**
    -   Indica que um servidor conectado enviou um pacote de dados (uma requisição).
    -   O `DBSrv` lê os dados do socket e os encaminha para a função `ProcessClientMessage` para tratamento.

-   **`WSA_READADMIN` (Dados Recebidos de um Administrador):**
    -   Indica que um cliente de admin enviou um comando.
    -   Os dados são lidos e encaminhados para a função `ProcessAdminMessage`.

## Outras Mensagens

-   **`WM_TIMER`:** Disparada a cada segundo para executar tarefas de manutenção (`ProcessSecTimer`).
-   **`WM_PAINT`:** Redesenha a janela, chamando `DrawConfig()` para exibir o status atual das conexões.
-   **`WM_CLOSE` / `WM_DESTROY`:** Gerencia o desligamento seguro do servidor.
