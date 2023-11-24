# Diretrizes para contribuidores

Esse arquivo é um resumo da estratégia de branch que vamos (tentar) seguir para que não haja mais problemas na hora de dar merge e de consertar certos aspectos do código. Eu nunca apliquei essa estratégia na prática então eu espero que funcione e caso vocês tenham alguma dúvida ou talvez uma estratégia melhor para a gente estar seguindo é só falar! =)

## Branches e suas funções

- **develop**: É o branch onde vai estar acontecendo o desenvolvimento do projeto, porém nós não vamos dar commit diretamente neste branch, e sim vamos criar dois subbranches cada vez que formos acrescetar algo no projeto:
	- **feature**: Toda vez que formos adicionar um recurso no nosso projeto nós vamos criar um novo branch a partir do _develop_ onde esse
 branch terá o nome do recurso que vamos desenvolver, por exemplo: _feature/ignorar-linhas-em-branco_.
	- **hotfix**: Seria a mesma mecânica do branch _feature_ porém aqui seria exclusivo para correção de erros, portanto seria a mesma nomenclatura: _hotfix/erro-de-runtime_. 
- **main**: É (obviamente) o branch principal, mas conforme eu falei antes o desenvolvimento vai acontecer mesmo no _develop_, nós só vamos dar merge no _develop_ -> _main_ quando tivermos uma versão pronta pra produção (também chamado de FUNCIONANDO).
	- **bugfix**: Esse branch vai ser usado quando alguém descobrir um erro no código do interpretador em produção, ou seja vai ser um branch do _main_ que de certa forma vai ter uma urgência pra ser corrigido. 

## Compilação

Como o interpretador é apenas um arquivo em C qualquer compilador de C ou C++ vai estar compilando ele normalmente, portanto sintam-se livres pra usar o **g++, gcc, o compilador da Microsoft, etc**.