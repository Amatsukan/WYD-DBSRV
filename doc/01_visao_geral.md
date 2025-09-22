# 1. Visão Geral e Arquitetura

O `DBSrv` é uma aplicação Windows que atua como o servidor central de banco de dados para um cluster de servidores de um jogo online. Ele é responsável por gerenciar todas as operações persistentes, como dados de contas, personagens, guilds e itens.

## Arquitetura Principal

A arquitetura é baseada em um modelo de **"hub and spoke"**:

-   **Hub (Centro):** O `DBSrv` é o componente central.
-   **Spokes (Raios):** Os vários servidores do jogo (servidores de mapa, login, etc.) se conectam ao `DBSrv` para validar informações e salvar o progresso dos jogadores.

Toda a comunicação é feita de forma assíncrona usando a API do Windows (WSAAsyncSelect), onde o servidor reage a eventos de rede em vez de bloquear a execução para esperar por dados.
