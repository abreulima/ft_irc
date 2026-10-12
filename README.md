# Tasks

- [x] KICK
- [x] PRIVMSG
- [x] TOPIC
- [x] MODE (leia a secção)
- [ ] Nick (parcialmente, devido ao HexChat).
- [ ] Join (Parcialmente, falta pedir senha, por limite, ou apenas entrar por convite).
- [ ] Invite
- [ ] Nao permitir que dois usuarios utilizam o mesmo Nickname *
- [ ] Parsing do argc, argv

\* A solução é um pouco trabalhosa porque o HexChat não encerra a conexão e tenta com _Second choice..._ ele usa o /NICK várias vezes até conseguir na mesma conexão.

## Modes

O parser do /MODE **deve** funcionar, mas nem tudo que depende dele, como o /JOIN por exemplo, está implementado.

## Crítico

As vezes o servidor fica stalling (parece que está a espera de algo e não processa as entradas),  **eu (Ivan)** creio que isso é causada quando dois usuários possuem o mesmo *Nickname*.

O parsing não é individual e o subject avisa que isso será testado, em nosso projeto atual, não é pessoal enviar mensagens incompletas e depois voltar a enviar.

Também há bastante acesso a memórias através de *maps* e *vectors*, eu estive a colocar algumas proteções.

O HexChat adiciona um delay (e parece que o servidor está lendo) caso o cliente esteja a enviar várias mensagens ao mesmo tempo, é possível remover com o comando ``/set net_throttle 0``.

## Mini Doc.

Retorna um vector com todos os canais que o *Client c* está conectado. Utilizado nos comandos /KICK e /NICK para informar todos os demais clientes que estão conectados ao mesmo canal sobre alguma mudança.
```cpp
std::vector<Channel*> Server::GetChannelsByClient(Client *c);
```

O comando /MODE pode conter vários argumentos ao mesmo tempo, e em qualquer odem, e a depender ser ele é aditivo ou não (começa com + ou -), o número de argumentos muda.

``/MODE +otlk Ivan 10 pizza``

O usuário *Ivan* passa ser um operador do canal  por causa do **o**.
Apenas operadores podem mudar o tópico por causa de **t**.
O limite de clientes no canal passa a ser 10 por causa de **l**.
Para aceder ao canal é necessário informar a senha "pizza"  por causa de **k**.

``/MODE -otlk Ivan``

O usuário *Ivan* passa ser um operador do canal, por causa do **o**.
Qualquer cliente do canal pode mudar o tópico.
O canal deixa de ter limite de usuários.
O canal não tem mais senha.

```cpp
struct Mode
{
    bool isOperation;
    bool isAdding;
    std::string channel;
    std::string modes;
    //std::vector<std::string> args;
    std::map<char, std::string> args;
};
```

Aceder ``args['k']`` devolve a a senha fornecida pelo usuário.