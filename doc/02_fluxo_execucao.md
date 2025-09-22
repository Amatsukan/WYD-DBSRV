# 2. Ponto de Entrada e Fluxo de Execução (`WinMain`)

A função `WinMain` é o ponto de entrada da aplicação e orquestra todo o processo de inicialização do servidor.

## Passos da Inicialização

1.  **Criação da Janela:** Uma janela principal é criada usando a API do Windows. Esta janela não serve para interação do usuário, mas sim como um **monitor de status** em tempo real e para receber mensagens de eventos de rede.

2.  **Inicialização de Rede:** O Winsock (`WSAInitialize`) é iniciado para habilitar a comunicação por sockets.

3.  **Carregamento de Dados e Configurações:** O servidor lê vários arquivos essenciais do disco para carregar seu estado inicial:
    -   `Config.txt`: Configurações gerais.
    -   `ServerList.txt`: Lista de servidores do jogo e seus IPs. É crucial para o `DBSrv` saber quais servidores podem se conectar a ele.
    -   `Admin.txt`: Lista de IPs com permissão de administrador.
    -   `BaseMob/`: Arquivos com status base das classes de personagem.
    -   `common/item.txt`: Definições de todos os itens do jogo.
    -   `Guilda.bin`: Informações sobre as guilds.

4.  **Abertura de Portas (Listeners):** O servidor começa a escutar por conexões em duas portas distintas:
    -   `DB_PORT`: Para os servidores do jogo.
    -   `ADMIN_PORT`: Para os clientes de administração.

5.  **Loop de Mensagens:** Após a inicialização, o servidor entra no loop de mensagens principal do Windows (`while(GetMessage(...))`). Ele permanece neste estado, processando eventos de forma assíncrona até que um comando de desligamento seja recebido.
