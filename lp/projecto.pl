% ist1106505 -> Rodrigo Salgueiro

:- set_prolog_flag(answer_write_options,[max_depth(0)]).
:- [dados], [keywords].

/*
CODIGO
*/

/*
DADOS
*/

% Os dados em dados.pl estao estruturados da seguinte forma:
% evento(ID, NomeDisciplina, Tipologia, NumAlunos, Sala)
% turno(ID, SiglaCurso, Ano, NomeTurma)
% horario(ID, DiaSemana, HoraInicio, HoraFim, Duracao, Periodo)

/*
QUALIDADE DOS DADOS
*/

eventosSemSalas(EventosSemSala) :-
    /*
    eventosSemSalas(EventosSemSala) e verdade se EventosSemSala e uma lista, 
    ordenada e sem elementos repetidos, de IDs de eventos sem sala.
    */
    findall(ID, evento(ID , _ , _ , _ , semSala), EventosSemSala).

eventosSemSalasDiaSemana(DiaDaSemana, EventosSemSala) :-
    /*
    eventosSemSalasDiaSemana(DiaDaSemana, EventosSemSala) e verdade se
    EventosSemSala e uma lista, ordenada e sem elementos repetidos, de IDs de 
    eventos sem sala que decorrem em DiaDaSemana.
    */
    findall(ID, (evento(ID , _ , _ , _ , semSala), 
    horario(ID, DiaDaSemana , _ , _ , _ , _ )), EventosSemSala).

% eventosSemSalasPeriodo
/*
eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala) e verdade se 
ListaPeriodos e uma lista de periodos e EventosSemSala e uma
lista, ordenada e sem elementos repetidos, de IDs de eventos sem sala nos 
periodos de ListaPeriodos.
*/
eventosSemSalasPeriodo([], []) :- !.

eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala) :-
    verifLPeriodos(ListaPeriodos), !,   
    % se o evento nao tiver sala e tiver um horario especificado na nossa
    % ListaPeriodos, adicionamos o seu ID aos EventosSemSala.
    findall(ID, (evento(ID , _ , _ , _ , semSala), 
    hrPeriodo(ID, ListaPeriodos)) , EventosSemSala).

verifLPeriodos(ListaPeriodos) :-
    /*
    verifLPeriodos(ListaPeriodos) e verdade se ListaPeriodos for uma lista de
    Periodos e um predicado auxiliar ao eventosSemSalasPeriodo.
    */
    member(p1, ListaPeriodos);
    member(p2, ListaPeriodos);
    member(p3, ListaPeriodos);
    member(p4, ListaPeriodos).

hrPeriodo(ID, ListaPeriodos) :-
    /*
    hrPeriodo(ID, ListaPeriodos) e verdade se ID for um identificador de um 
    determinado evento, que ocorra num periodo especificado na lista
    ListaPeriodos. E um predicado auxiliar ao eventosSemSalasPeriodo.
    */
    member(p1, ListaPeriodos),
    (horario(ID, _ , _ , _ , _, p1), !; horario(ID, _ , _ , _ , _, p1_2), !);
    member(p2, ListaPeriodos),
    (horario(ID, _ , _ , _ , _, p2), !; horario(ID, _ , _ , _ , _, p1_2), !);
    member(p3, ListaPeriodos),
    (horario(ID, _ , _ , _ , _, p3), !; horario(ID, _ , _ , _ , _, p3_4), !);
    member(p4, ListaPeriodos),
    (horario(ID, _ , _ , _ , _, p4), !; horario(ID, _ , _ , _ , _, p3_4), !).


/*
PESQUISAS SIMPLES
*/

% organizaEventos
/*
organizaEventos(ListaEventos, Periodo, EventosNoPeriodo) e verdade se
EventosNoPeriodo e a lista, ordenada e sem elementos repetidos, de IDs dos
eventos de ListaEventos que ocorrem no periodo 'Periodo'.
*/

organizaEventos(ListaEventos, Periodo, EventosNoPeriodo) :-
    % vamos criar uma predicado organizaEventos/4 para termos um acumulador
    organizaEventos(ListaEventos, Periodo, [], EventosNoPeriodo2), !,
    sort(EventosNoPeriodo2, EventosNoPeriodo).

% quando acabamos de circular a lista, a nossa lista Acc podera unificar
% com o EventosNoPeriodo
organizaEventos([], _, EventosNoPeriodo, EventosNoPeriodo).

organizaEventos([ID1 | Resto], Periodo, Acc, EventosNoPeriodo) :-
    % caso ID1 faca parte do periodo escolhido, adicionamos a lista Acc
    hrPeriodo(ID1, [Periodo]),
    organizaEventos(Resto, Periodo, [ID1 | Acc], EventosNoPeriodo).

organizaEventos([_ | Resto], Periodo, Acc, EventosNoPeriodo) :-
    organizaEventos(Resto, Periodo, Acc, EventosNoPeriodo).


% eventosMenoresQue
/*
eventosMenoresQue(Duracao, ListaEventosMenoresQue) e verdade se 
ListaEventosMenoresQue e a lista ordenada e sem elementos repetidos dos
identificadores dos eventos que tem duracao menor ou igual a Duracao.
*/

eventosMenoresQue(Duracao, ListaEventosMenoresQue) :-
    findall(ID, (horario(ID, _, _, _, DurAt, _), DurAt =< Duracao),
    ListaEventosMenoresQue).


% eventosMenoresQueBool
/*
eventosMenoresQueBool(ID, Duracao) e verdade se o evento identificado por ID
tiver duracao igual ou menor a Duracao.
*/

eventosMenoresQueBool(ID, Duracao) :-
    horario(ID, _, _, _, DurAt, _),
    DurAt =< Duracao.

% procuraDisciplinas
/*
procuraDisciplinas(Curso, ListaDisciplinas) e verdade se ListaDisciplinas e a
lista ordenada alfabeticamente do nome das disciplinas do curso Curso.
*/

procuraDisciplinas(Curso, ListaDisciplinas) :-
    findall(Disciplina, (turno(ID, Curso, _, _), 
    evento(ID, Disciplina, _, _, _)), ListaNOrd),
    sort(ListaNOrd, ListaDisciplinas).

% organizaDisciplinas
/*
organizaDisciplinas(ListaDisciplinas, Curso, Semestres) e verdade se Semestres
e uma lista com duas listas, em que a primeira lista contem as disciplinas da
ListaDisciplinas (do Curso) que fazem parte do primeiro semestre, e a segunda
lista contem as que que fazem parte do segundo semestre.
*/

organizaDisciplinas(ListaDisciplinas, Curso, [PSemestre, SSemestre]) :-
    % vamos criar uma predicado organizaDisciplinas/4 para termos um acumulador,
    % que inicialmente contem 2 listas vazias (que unificarao com
    % PSemestre e SSemestre)    
    organizaDisciplinas(ListaDisciplinas, Curso, [[],[]],
    [PSemestreNO, SSemestreNO]), !,
    sort(PSemestreNO, PSemestre),
    sort(SSemestreNO, SSemestre).

% quando limpamos a ListaDisciplinas, chegamos ao resultado desejado
organizaDisciplinas([], _, Semestres, Semestres).

organizaDisciplinas([PDisciplina | Resto], Curso, [PSemestre, SSemestre], 
Semestres) :-
    % vamos encontrar o periodo da primeira disciplina da lista 
    % ListaDisciplinas
    evento(ID, PDisciplina, _, _, _),
    turno(ID, Curso, _, _),
    horario(ID, _, _, _, _, Periodo),

    % se o Periodo for um destes, a disciplina e do primeiro semestre
    ((member(Periodo, [p1, p2, p1_2]),
    organizaDisciplinas(Resto, Curso, [[PDisciplina | PSemestre], SSemestre],
    Semestres), !);

    % caso contrario, a disciplina e do segundo semestre
    organizaDisciplinas(Resto, Curso, [PSemestre, [PDisciplina | SSemestre]],
    Semestres), !).

% horasCurso
/*
horasCurso(Periodo, Curso, Ano, TotalHoras) e verdade se TotalHoras for o 
numero de horas total dos eventos associadas ao curso Curso, no ano Ano e 
periodo Periodo.
*/

horasCurso(Periodo, Curso, Ano, TotalHoras) :-
    findall(ID, (turno(ID, Curso, Ano, _), hrPeriodo(ID, [Periodo])), 
    ListaCRep),
    % sort para removermos os elementos repetidos
    sort(ListaCRep, ListaID),
    findall(Duracao, (member(ID, ListaID), horario(ID, _, _, _, Duracao, _)), 
    ListaHoras),
    sumlist(ListaHoras, TotalHoras).

% evolucaoHorasCurso

evolucaoHorasCurso(Curso, Evolucao) :-
    /*
    evolucaoHorasCurso(Curso, Evolucao) e verdade se Evolucao for uma lista de
    tuplos na forma (Ano, Periodo, NumHoras), em que NumHoras e o total de horas
    associadas ao curso Curso, no ano Ano e periodo Periodo.
    */
    ListaAnos = [1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3],
    ListaPeriodos = [p1, p2, p3, p4, p1, p2, p3, p4, p1, p2, p3, p4],
    maplist(periodoCriador(Curso), ListaAnos, ListaPeriodos, Evolucao).

periodoCriador(Curso, Ano, Periodo, Tuplo) :-
    /*
    periodoCriador(Curso, Ano, Periodo, Tuplo) e uma predicado auxiliar que e
    verdade caso Tuplo seja o tuplo da forma (Ano, Periodo, NumHoras), em
    que NumHoras e o numero de horas de eventos nesse periodo desse ano.
    */
    horasCurso(Periodo, Curso, Ano, HorasPeriodo),
    Tuplo = (Ano, Periodo, HorasPeriodo).

% ocupaSlot
/*
ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas)
e verdade se Horas for o numero de horas sobrepostas entre o evento que tem 
inicio em HoraInicioEvento e fim em HoraFimEvento, e o slot que tem inicio em
HoraInicioDada e fim em HoraFimDada.
*/

ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, 
HoraFimEvento, Horas) :-
    % caso o evento esteja sobreposto pelo slot
    ((HoraInicioDada =< HoraInicioEvento, HoraFimDada >= HoraFimEvento,
    Horas is HoraFimEvento - HoraInicioEvento, !);

    % caso o slot esteja totalmente contido no evento
    (HoraInicioDada >= HoraInicioEvento, HoraFimDada =< HoraFimEvento,
    Horas is HoraFimDada - HoraInicioDada, !);

    % caso o slot esteja contido parcialmente no evento (com uma parte
    % antes da hora inicial)
    (HoraInicioDada =< HoraInicioEvento, HoraFimDada =< HoraFimEvento,
    Horas is HoraFimDada - HoraInicioEvento, !);

    % caso o slot esteja contido parcialmente no evento (com uma parte
    % depois da hora final)
    (HoraInicioDada >= HoraInicioEvento, HoraFimDada >= HoraFimEvento,
    Horas is HoraFimEvento - HoraInicioDada, !)),

    Horas > 0.

% numHorasOcupadas
/*
numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, SomaHoras)
e verdade se SomaHoras for o numero de horas ocupadas nas salas do tipo 
TipoSala, no intervalo de tempo definido entre HoraInicio e HoraFim, no dia da
semana DiaSemana, e no periodo Periodo.
*/

numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, 
HoraFim, SomaHoras) :-

    % criamos a lista Salas, com todas as salas do tipo TipoSala
    salas(TipoSala, Salas),

    % encontra todos os IDs de eventos que sejam realizados numa Sala do
    % TipoSala, no dia de semana DiaSemana, durante o periodo Periodo
    findall(ID, (evento(ID, _, _, _, Sala), horario(ID, DiaSemana, _, _, _, _),
    hrPeriodo(ID, [Periodo]), member(Sala, Salas)), ListaIDs),
    write(ListaIDs),

    % vamos agora criar 2 listas, uma com as horas inicio de cada evento,
    % e outra com as horas fim
    maplist(descobreHorasInicio, ListaIDs, HorasInicioIDs),
    maplist(descobreHorasFim, ListaIDs, HorasFimIDs),

    % vamos agora aplicar o ocupaSlotMod a nossa Lista de IDs, para perceber
    % o numero de horas ocupadas de cada sala no horario especificado
    maplist(ocupaSlotMod(HoraInicio, HoraFim), HorasInicioIDs, HorasFimIDs,
    LHorasOcupadas),
    write(LHorasOcupadas),

    % por fim, somamos todos os elementos da lista
    sumlist(LHorasOcupadas, SomaHoras), !.

descobreHorasInicio(ID, HoraInicioEvento) :-
    /*
    descobreHorasInicio(ID, HoraInicioEvento) e verdade se HoraInicioEvento
    for correspondente a hora inicial do evento de id ID.
    */
    horario(ID, _, HoraInicioEvento, _, _, _).

descobreHorasFim(ID, HoraFimEvento) :-
    /*
    descobreHorasFim(ID, HoraFimEvento) e verdade se HoraFimEvento for 
    corresponde a hora final do evento de id ID.
    */
    horario(ID, _, _, HoraFimEvento, _, _).

ocupaSlotMod(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas) :-
    /*
    ocupaSlotMod(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento,
    Horas) e uma modificacao do predicado original ocupaSlot, que retorna 0 como
    Horas caso o resultado da predicado original seja false (visto que nao
    faz sentido conter elementos negativos caso a interseccao nao exista).
    */
    ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas);
    Horas = 0.

% ocupacaoMax
/*
ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max) e verdade se Max for o numero
de horas possiveis de ser ocupadas por salas do tipo TipoSala, no intervalo de
tempo definido entre HoraInicio e HoraFim.
*/

ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max) :-

    IntrvTempo is HoraFim - HoraInicio,

    % descobrimos quantas salas desse tipo existem
    salas(TipoSala, Salas),
    length(Salas, NSalas),

    % resolucao da expressao dada
    Max is IntrvTempo * NSalas.

% percentagem
/*
percentagem(SomaHoras, Max, Percentagem) e verdade se Percentagem for a divisao
de SomaHoras por Max, multiplicada por 100.
*/

percentagem(SomaHoras, Max, Percentagem) :-
    Percentagem is (SomaHoras / Max) * 100.

% ocupacaoCritica

ocupacaoCritica(HoraInicio, HoraFim, Threshold, Resultados) :-
    Periodos = [p1, p2, p3, p4],
    % vamos utilizar um findall para chegar ao resultado, em todos os periodos.
    % para isso, teremos que aplicar condicoes que satisfacam tudo o que esta
    % descrito no enunciado. ou seja, queremos os casosCriticos(DiaSemana,
    % TipoSala, Percentagem). Para isso, vamos buscar os IDs dos eventos 
    % realizados nesse tipo de salas, depois vamos buscar os dias em que se
    % ocorrem, e os seu horarios inicial e final. Posteriormente vamos ver
    % se ha sobreposicao entre os horarios dados como argumento, e os horarios
    % dos eventos. caso haja, prosseguimos e chegamos a soma de horas que
    % todos esses eventos ocupam. A partir dai, basta fazer a percentagem
    % dessa ocupacao relativamente a ocupacao maxima e chegamos a nossa
    % lista Resultado.
    findall(casosCriticos(DiaSemana, TipoSala, Percentagem),(salas(TipoSala, 
    Salas), member(Periodo, Periodos), evento(ID, _, _, _, SalaEv), 
    member(SalaEv, Salas), horario(ID, DiaSemana, _, _, _, Periodo),
    numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim,
    SomaHoras), ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max), percentagem(
    SomaHoras, Max, Perctg), Perctg > Threshold, Percentagem is 
    ceiling(Perctg)), Result),
    % remover resultados repetidos e ordenar
    sort(Result, Resultados).


/*
AND NOW FOR SOMETHING COMPLETELY DIFFERENT...
*/

% ocupacaoMesa

/*
ocupacaoMesa(ListaPessoas, ListaRestricoes, OcupacaoMesa) e verdade se
ListaPessoas for a lista com o nome das pessoas a sentar a mesa, ListaRestricoes
for a lista de restricoes a verificar e OcupacaoMesa for uma lista com tres
listas, em que a primeira contem as pessoas de um lado da mesa, a segunda as 
pessoas a cabeceira e a terceira as pessoas do outro lado da mesa, de modo a 
que essas pessoas sejam exactamente as da ListaPessoas e verifiquem todas as
restricoes de ListaRestricoes.
*/

ocupacaoMesa(ListaPessoas, ListaRestricoes, OcupacaoMesa) :-
    % criaremos uma lista com todas as alternativas possiveis para sentar estas
    % 8 pessoas usando o geraMesasPossiveis. de seguida, entramos na nossa
    % recursiva
    geraMesasPossiveis(ListaPessoas, Mesas),
    ocupacaoMesa(ListaPessoas, ListaRestricoes, Mesas, OcupacaoMesa).

ocupacaoMesa(_, [], MesasSFormat, Mesas) :-
    % quando acabamos as restricoes, o resultado esta pronto. e apenas preciso
    % transformar a lista "flat" numa lista dividida por zona da mesa.
    append(MesasSFormat, Mesas2),
    transfFlatEmForm(Mesas2, Mesas).

ocupacaoMesa(ListaPessoas, [PRestricao | Resto], Mesas, OcupacaoMesa) :-
    % a nossa recursiva avaliara cada uma das restricoes de forma recursiva,
    % resolvendo cada uma e apagando as posteriormente, ate chegar a lista
    % vazia.

    % cab1(X)
    (functor(PRestricao, cab1, 1),
    arg(1, PRestricao, Pessoa),
    findall(Mesa, (member(Mesa, Mesas), nth1(4, Mesa, Pessoa)), Mesas2),
    ocupacaoMesa(ListaPessoas, Resto, Mesas2, OcupacaoMesa));

    % cab2(X)
    (functor(PRestricao, cab2, 1),
    arg(1, PRestricao, Pessoa),
    findall(Mesa, (member(Mesa, Mesas), nth1(5, Mesa, Pessoa)), Mesas2),
    ocupacaoMesa(ListaPessoas, Resto, Mesas2, OcupacaoMesa));
    
    % honra(X,Y)
    (functor(PRestricao, honra, 2),
    arg(1, PRestricao, Pessoa1), arg(2, PRestricao, Pessoa2),
    findall(Mesa, (member(Mesa, Mesas), honraBool(Pessoa1, Pessoa2, Mesa)),
    Mesas2),
    ocupacaoMesa(ListaPessoas, Resto, Mesas2, OcupacaoMesa));
    
    % lado(X,Y)
    (functor(PRestricao, lado, 2),
    arg(1, PRestricao, Pessoa1), arg(2, PRestricao, Pessoa2),
    findall(Mesa, (member(Mesa, Mesas), ladoBool(Pessoa1, Pessoa2, Mesa)), 
    Mesas2),
    ocupacaoMesa(ListaPessoas, Resto, Mesas2, OcupacaoMesa));

    % naoLado(X,Y)
    (functor(PRestricao, naoLado, 2),
    arg(1, PRestricao, Pessoa1), arg(2, PRestricao, Pessoa2),
    findall(Mesa, (member(Mesa, Mesas), not(ladoBool(Pessoa1, Pessoa2, Mesa))), 
    Mesas2),
    ocupacaoMesa(ListaPessoas, Resto, Mesas2, OcupacaoMesa));

    % frente(X,Y)
    (functor(PRestricao, frente, 2),
    arg(1, PRestricao, Pessoa1), arg(2, PRestricao, Pessoa2),
    findall(Mesa, (member(Mesa, Mesas), frenteBool(Pessoa1, Pessoa2, Mesa)), 
    Mesas2),
    ocupacaoMesa(ListaPessoas, Resto, Mesas2, OcupacaoMesa));

    % naoFrente(X,Y)
    (functor(PRestricao, naoFrente, 2),
    arg(1, PRestricao, Pessoa1), arg(2, PRestricao, Pessoa2),
    findall(Mesa, (member(Mesa, Mesas), not(frenteBool(Pessoa1, Pessoa2, Mesa))), 
    Mesas2),
    ocupacaoMesa(ListaPessoas, Resto, Mesas2, OcupacaoMesa)).


honraBool(Pessoa1, Pessoa2, Mesa) :-
    /*
    honraBool(Pessoa1, Pessoa2, Mesa) e o predicado auxiliar que e verdade se
    Pessoa1 e Pessoa2 corresponderem aos principios Honra descritos - ou seja,
    estar a Pessoa1 sentada numa das cabeceiras e a Pessoa2 a sua direita.
    */
    (nth1(4, Mesa, Pessoa1), nth1(6, Mesa, Pessoa2)); 
    (nth1(5, Mesa, Pessoa1), nth1(3, Mesa, Pessoa2)).

ladoBool(Pessoa1, Pessoa2, Lista) :-
    /*
    ladoBool(Pessoa1, Pessoa2, Mesa) e o predicado auxiliar que e verdade se
    Pessoa1 e Pessoa2 corresponderem aos principios Lado descritos - ou seja,
    estarem Pessoa1 e Pessoa2, lado a lado, na mesa.
    */
    nth0(Pos, Lista, Pessoa1),
    PosEsq is Pos - 1,
    PosDir is Pos + 1,
    (nth0(PosEsq, Lista, Pessoa2) ; nth0(PosDir, Lista, Pessoa2)), !.

frenteBool(Pessoa1, Pessoa2, Lista) :-
    /*
    frenteBool(Pessoa1, Pessoa2, Mesa) e o predicado auxiliar que e verdade se
    Pessoa1 e Pessoa2 corresponderem aos principios Frente descritos - ou seja,
    estarem Pessoa1 e Pessoa2, frente a frente, na mesa.
    */
    nth1(Pos, Lista, Pessoa1),
    % lista de quem esta a frente de quem -> X6 esta a frente de X1, por ex
    ListaFr = [6,7,8,5,4,1,2,3],
    nth1(Pos, ListaFr, PosFr),
    % verificar se a Pessoa2 esta nessa posicao
    nth1(PosFr, Lista, Pessoa2), !.

geraMesasPossiveis(ListaPessoas, CombinacaoMesas) :-
    /*
    geraMesasPossiveis(ListaPessoas, MesaRandom) e o predicado auxiliar que e
    verdade se CombinacaoMesas representar todas as combinacoes possiveis de 
    sentar as pessoas de ListaPessoas numa mesa.
    */
    findall(Hip, permutation(ListaPessoas, Hip), CombinacaoMesas).

transfFlatEmForm([X1, X2, X3, X4, X5, X6, X7, X8], LstForm) :-
    /*
    transfFlatEmForm([a, b, c, d, e, f, g, h], LstForm) e o predicado auxiliar que
    e verdade se LstForm representar a lista formatada a partir da primeira.
    Aqui por formatacao entende-se adequar a lista a mesa, ou seja, criar 3
    sublistas em que cada uma delas representa uma parte da mesa.
    */
    LstForm = [[X1, X2, X3], [X4, X5], [X6, X7, X8]].


/*
FIM
*/