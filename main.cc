
#include <drogon/drogon.h>
#include <drogon/orm/DbConfig.h>
#include <cstdlib>
#include <iostream>

int main() {
    const char *senha = std::getenv("DB_PASSWORD");

    if (senha == nullptr) {
        std::cerr << "Erro: DB_PASSWORD nao definida!" << std::endl;
        return 1;
    }

    drogon::orm::PostgresConfig config{};

    config.host = "postgres-db";
    config.port = 5432;
    config.databaseName = "notes_db";
    config.username = "drogon_user";
    config.password = senha;
    config.connectionNumber = 1;
    config.name = "default";
    config.isFast = false;
    config.timeout = -1.0;
    config.autoBatch = false;

    drogon::app().addDbClient(config);

    drogon::app().addListener("0.0.0.0", 8080);


    drogon::app().registerHandler(
        "/livros",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {

            auto db = drogon::app().getDbClient();

            db->execSqlAsync(
                "SELECT id, titulo, autor, ano_publicacao, isbn FROM livros ORDER BY id",
                [callback](const drogon::orm::Result &resultado) {
                    Json::Value livros(Json::arrayValue);

                    for (const auto &linha : resultado) {
                        Json::Value livro;
                        livro["id"] = linha["id"].as<int>();
                        livro["titulo"] = linha["titulo"].as<std::string>();
                        livro["autor"] = linha["autor"].as<std::string>();
                        livro["ano_publicacao"] = linha["ano_publicacao"].isNull()
                            ? 0 : linha["ano_publicacao"].as<int>();
                        livro["isbn"] = linha["isbn"].isNull()
                            ? "" : linha["isbn"].as<std::string>();

                        livros.append(livro);
                    }

                    auto resposta = drogon::HttpResponse::newHttpJsonResponse(livros);
                    callback(resposta);
                },
                [callback](const drogon::orm::DrogonDbException &erro) {
                    auto resposta = drogon::HttpResponse::newHttpResponse();
                    resposta->setStatusCode(drogon::k500InternalServerError);
                    resposta->setBody("Erro ao consultar os livros.");
                    callback(resposta);
                }
            );
        },
        {drogon::Get}
    );


    // Rota para cadastrar um novo livro
    // Rota para cadastrar um novo livro
drogon::app().registerHandler(
    "/cadastrar-livro",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {

            auto dados = req->getJsonObject();

            if (!dados ||
                !(*dados).isMember("titulo") ||
                !(*dados).isMember("autor") ||
                !(*dados)["titulo"].isString() ||
                !(*dados)["autor"].isString() ||
                (*dados)["titulo"].asString().empty() ||
                (*dados)["autor"].asString().empty()) {

                auto resposta = drogon::HttpResponse::newHttpResponse();
                resposta->setStatusCode(drogon::k400BadRequest);
                resposta->setBody("Titulo e autor sao obrigatorios.");
                callback(resposta);
                return;
            }

            std::string titulo = (*dados)["titulo"].asString();
            std::string autor = (*dados)["autor"].asString();
            int ano = (*dados).get("ano_publicacao", 0).asInt();
            std::string isbn = (*dados).get("isbn", "").asString();

            auto db = drogon::app().getDbClient();

            db->execSqlAsync(
                "INSERT INTO livros (titulo, autor, ano_publicacao, isbn) "
                "VALUES ($1, $2, $3, $4) RETURNING id",
                [callback](const drogon::orm::Result &resultado) {
                    Json::Value respostaJson;
                    respostaJson["mensagem"] = "Livro cadastrado com sucesso!";
                    respostaJson["id"] = resultado[0]["id"].as<int>();

                    auto resposta =
                        drogon::HttpResponse::newHttpJsonResponse(respostaJson);
                    resposta->setStatusCode(drogon::k201Created);
                    callback(resposta);
                },
                [callback](const drogon::orm::DrogonDbException &erro) {
                    auto resposta = drogon::HttpResponse::newHttpResponse();
                    resposta->setStatusCode(drogon::k500InternalServerError);
                    resposta->setBody("Erro ao cadastrar livro.");
                    callback(resposta);
                },
                titulo, autor, ano, isbn
            );
        },
        {drogon::Post}
    );


    // Rota para excluir um livro pelo ID
    drogon::app().registerHandler(
        "/excluir-livro/{1}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback,
           int id) {

            auto db = drogon::app().getDbClient();

            db->execSqlAsync(
                "DELETE FROM livros WHERE id = $1",
                [callback](const drogon::orm::Result &resultado) {
                    Json::Value respostaJson;
                    respostaJson["mensagem"] = "Livro excluido com sucesso!";

                    auto resposta =
                        drogon::HttpResponse::newHttpJsonResponse(respostaJson);
                    callback(resposta);
                },
                [callback](const drogon::orm::DrogonDbException &erro) {
                    auto resposta = drogon::HttpResponse::newHttpResponse();
                    resposta->setStatusCode(drogon::k500InternalServerError);
                    resposta->setBody("Erro ao excluir livro.");
                    callback(resposta);
                },
                id
            );
        },
        {drogon::Delete}
    );

    drogon::app().run();


    return 0;
}



