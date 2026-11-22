# 📚 Minha Biblioteca

Sistema web para gerenciamento de livros, desenvolvido como atividade acadêmica da disciplina de Banco de Dados.

## 🎯 Objetivo

Desenvolver uma aplicação web integrada a um banco de dados relacional, permitindo realizar operações de cadastro, consulta e exclusão de livros.

## 💻 Tecnologias utilizadas

- **C++:** linguagem utilizada no desenvolvimento do backend.
- **Drogon:** framework C++ responsável pelo servidor web e pelas rotas HTTP.
- **PostgreSQL:** banco de dados relacional utilizado para armazenar os livros.
- **Docker:** utilizado para executar os ambientes do Drogon e do PostgreSQL.
- **HTML, CSS e JavaScript:** utilizados na construção da interface web.

## ⚙️ Funcionalidades

- Cadastrar livros informando título, autor, ano de publicação e ISBN.
- Listar os livros cadastrados.
- Excluir livros mediante confirmação.
- Armazenar e consultar informações no PostgreSQL.

## 🗄️ Estrutura do banco de dados

A aplicação utiliza uma tabela chamada `livros`.

```sql
CREATE TABLE livros (
    id SERIAL PRIMARY KEY,
    titulo VARCHAR(150) NOT NULL,
    autor VARCHAR(100) NOT NULL,
    ano_publicacao INTEGER,
    isbn VARCHAR(20)
);
```

## 🚀 Execução

Para executar o projeto, é necessário ter o Docker configurado com um ambiente Drogon e um servidor PostgreSQL.

A aplicação utiliza as seguintes configurações de banco de dados:

- Banco: `notes_db`
- Usuário: `drogon_user`
- Host: `postgres-db`
- Porta: `5432`

A senha é fornecida pela variável de ambiente `DB_PASSWORD`, sem ser armazenada no código-fonte.

Com os contêineres configurados e em execução, acesse o terminal do contêiner Drogon, configure a variável de ambiente e execute o programa compilado.

```bash
cd /app/biblioteca
./build/biblioteca
```

Depois, abra no navegador:

`http://localhost:8080`

**Observação:** os comandos acima correspondem ao ambiente Docker utilizado no desenvolvimento. Para executar em outro computador, é necessário configurar o banco de dados, as dependências e compilar o projeto.

## 📌 Rotas da aplicação

| Método | Rota | Descrição |
|---|---|---|
| GET | `/livros` | Consulta os livros cadastrados |
| POST | `/cadastrar-livro` | Cadastra um novo livro |
| DELETE | `/excluir-livro/{id}` | Exclui um livro pelo ID |

## 🎓 Contexto acadêmico

Projeto desenvolvido para aplicar conhecimentos de banco de dados, integração entre frontend e backend, linguagem C++ e comunicação com um sistema gerenciador de banco de dados relacional.