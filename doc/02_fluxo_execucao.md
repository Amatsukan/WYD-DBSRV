# 2. Ponto de Entrada e Fluxo de Execução (`main`)

A função `main` é o ponto de entrada da aplicação e orquestra todo o processo de inicialização do servidor. Como uma aplicação de console, todo o feedback e interação ocorrem via linha de comando.

## Passos da Inicialização

1.  **Criação do Objeto `Server`:** A aplicação cria uma instância do objeto `Server`, que encapsula toda a lógica do servidor.

2.  **Inicialização do Servidor (`server.init()`):** O método `init` é chamado para preparar o servidor para a execução.
    -   **Inicialização do Logger:** O sistema de log é iniciado para gravar eventos em `DBServidor.log`.
    -   **Carregamento de Dados e Configurações:** O servidor lê vários arquivos essenciais do disco:
        -   `Config.txt`: Configurações gerais.
        -   `Admin.txt`: Lista de IPs com permissão de administrador.
        -   O índice do servidor é determinado com base no `hostname` e no `serverlist.txt`.
        -   Dados de jogo como `Sapphire` e `LastCapsule` são carregados.
        -   O `DataManager` é inicializado, carregando informações de guilds e rankings.
    -   **Inicialização de Rede:** O `NetworkManager` é iniciado e as portas para os servidores do jogo (`DB_PORT`) e para administração (`ADMIN_PORT`) são abertas.
    -   **Inicialização de Tarefas:** O `TaskManager` é iniciado para executar tarefas periódicas.

3.  **Loop Principal (`server.run()`):** Após a inicialização, o servidor entra no seu loop principal.
    -   Uma thread separada é iniciada para lidar com a entrada do console (`handleConsoleInput`), permitindo que administradores digitem comandos.
    -   O servidor permanece em execução, processando conexões de rede e tarefas agendadas.

4.  **Desligamento (`server.shutdown()`):** Quando o comando `exit` é recebido, o processo de desligamento é iniciado.
    -   Os módulos de tarefas e rede são parados.
    -   Dados importantes, como configurações e informações de guilds, são salvos em disco.
    -   O logger é finalizado.