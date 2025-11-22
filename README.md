# Estacionamento Inteligente
Este projeto implementa um sistema de monitoramento em tempo real para uma vaga de estacionamento utilizando um microcontrolador **ESP32** e um sensor ultrassônico **HC-SR04**. Os dados do sensor são enviados via Wi-Fi para um servidor web (XAMPP/PHP), onde são processados, armazenados e exibidos em uma interface web responsiva e protegida por login.

## ✨ Funcionalidades

* **Monitoramento em Tempo Real:** Status da vaga atualizado a cada 5 segundos via AJAX.
* **Autenticação de Usuários:** Tela de login protegida por credenciais armazenadas em SQLite.
* **Gerenciamento de Funcionários (Admin):** Tela exclusiva para administradores cadastrarem novos usuários (funcionários) e visualizarem a lista completa.
* **Design Responsivo:** Interface otimizada para visualização em desktop e dispositivos móveis.
* **Sistema Robusto de Medição:** Utilização de média de 11 amostras com descarte de 3 outliers, debounce lógico de 3 ciclos (15 segundos) e histerese para ignorar ruído de baixa distância.

## 🛠️ Tecnologias Utilizadas

### Hardware
* **Microcontrolador:** ESP32.
* **Sensor:** HC-SR04 (Ultrassônico).

### Software (Backend & DB)
* **Servidor Web:** XAMPP (Apache + PHP).
* **Linguagem:** PHP 8.1.25.
* **Banco de Dados:** SQLite (para gerenciamento de usuários).
* **Comunicação:** JSON (entre ESP32 e PHP).

### Software (Frontend)
* **Estrutura:** HTML5 e CSS3.
* **Interatividade:** JavaScript (Fetch API para polling de dados).
## 📁 Estrutura do Projeto (MVC)
```
sensor/
├── index.php                     <-- Ponto de Entrada 
│   # Recebe todas as requisições (rotas: monitor, login, logout, manage_users) e 
│   # as direciona para o Controller apropriado.
│
├── receber.php                   <-- Endpoint do Sensor (Comunicação ESP32)
│   # Recebe dados JSON via POST do ESP32 e utiliza o DataModel para salvar em dados.json.
│
├── dados.json                    <-- Arquivo de Dados
│   # JSON simples que armazena o último status de leitura do sensor (ex: {"vaga":"Vaga 1","status":"OCUPADA"}).
│
├── users.sqlite                  <-- Banco de Dados SQLite
│   # Armazena as credenciais (hash) e cargos dos usuários.
│
├── init_db.php                   <-- Script de Inicialização (Executar apenas uma vez)
│   # Cria a tabela 'users', a coluna 'cargo' e insere o usuário 'admin' inicial.
│
├── assets/                       <-- Recursos Estáticos (CSS, Fontes, etc.)
│   ├── style.css                 
│   └── pageAdmin.css             
│
├── config/                       <-- Configurações
│   └── db.php                    # Função de conexão PDO para o banco de dados SQLite (users.sqlite).
│
├── controllers/                  <-- Lógica de Controle (Processamento de Rotas)
│   ├── AuthController.php        
│   ├── MonitorController.php     
│   └── UserController.php       
│
├── models/                       <-- Lógica de Negócios e Dados
│   ├── DataModel.php             
│   └── UserModel.php             
│
└── views/                        <-- Apresentação (Interface do Usuário)
    ├── _header.php              
    ├── login.php                 
    ├── monitor.php               
    └── user_manager.php          
```
