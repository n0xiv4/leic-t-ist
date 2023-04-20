'''
Nome: Rodrigo Salgueiro
IST_ID: ist1106505
Email: ist1106505@tecnico.ulisboa.pt
Última data de submissão: 27/10/2022
'''

'''
Tarefa 1: Justificação de Textos
'''

def limpa_texto(txt):
    '''
        corta_texto(txt) é a função que recebe uma cadeia de caracteres e
        devolve a cadeia de caracteres limpa, removendo os espaços brancos
        como descrito.
    '''
    txt = " ".join(txt.split())
    return txt


def corta_texto(txt, larg):
    '''
        corta_texto(txt, larg) é a função que recebe uma cadeia de
        caracteres e um inteiro positivo correspondentes a um texto limpo
        e uma largura de coluna correspondente, e que devolve duas subcadeias
        de caracteres limpas - a primeira contendo todas as palavras completas
        desde o ínicio da cadeia original até ao comprimento descrito, e a
        segunda contendo o resto do texto de entrada.
    '''
    char = -1
    txt = txt.split()
    for x in range(len(txt)):
        char += len(txt[x]) + 1
        if char > larg:
            txt_a = " ".join(txt[:x])
            txt_b = " ".join(txt[x:])
            return txt_a, txt_b

    if char <= larg:
        txt_a = " ".join(txt)
        txt_b = ""
        return txt_a, txt_b


def corta_texto_mod(txt, larg):
    '''
        corta_texto_mod(txt, larg) é a função auxiliar com a mesma função
        que a função corta_texto, com a diferença de que serve de adaptação
        ao justifica_texto.
    '''
    char = 0
    txt = txt.split()
    for x in range(len(txt)):
        if char == 0:
            char += len(txt[x])
        else:
            char += len(txt[x]) + 1
        if len(txt[0]) > larg:
            txt_a = " ".join(txt[:1])
            txt_b = " ".join(txt[1:])
            return txt_a, txt_b
        else:
            if char > larg:
                txt_a = " ".join(txt[:x])
                txt_b = " ".join(txt[x:])
                return txt_a, txt_b

    if char < larg:
        txt_a = " ".join(txt)
        txt_b = ""
        return txt_a, txt_b


def insere_espacos(txt, larg):
    '''
        insere_espacos(txt, larg) é a função que recebe uma cadeia de
        caracteres e um inteiro positivo correspondentes a um texto limpo
        e uma largura de coluna correspondente, e no caso da cadeia de
        entrada conter duas ou mais palavras, devolve uma cadeia de caracteres
        de comprimento igual à largura descrita.
    '''
    char = 0
    pos = 1
    vzs = 0
    txt = txt.split()

    if len(txt) != 1:
        for x in range(len(txt)):
            if char == 0:
                char += len(txt[x])
            else:
                char += len(txt[x]) + 1

        while char < larg:
            if pos == len(txt):
                pos = 1
                vzs += 1

            txt.insert(pos, '')
            char += 1
            pos += 2 + vzs

        txt = " ".join(txt)
        return txt

    else:
        txt = " ".join(txt)
        txt = txt.ljust(larg)
        return txt


def e_espaco_total(st):
    '''
        e_espaco_total(st) é a função auxiliar que verifica se uma string é
        composta apenas por espaços, retornando True nesse caso.
    '''
    for char in st:
        if char.isspace() is False:
            return False
    return True


def justifica_texto(txt, larg):
    '''
        justifica_texto(txt, larg) é a função que recebe uma cadeia de
        caracteres não vazia e um inteiro positivo correspondentes a um texto
        qualquer e uma largura de coluna respetivamente, devolvendo um tuplo
        de cadeias de caracteres justificadas.
    '''
    if (
        not ((isinstance(txt, str) and isinstance(larg, int))) or 
        e_espaco_total(txt) or not larg > 0
    ):
        raise ValueError('justifica_texto: argumentos invalidos')
    
    txt = txt.split()

    for plvr in txt:
        if len(plvr) > larg:
            raise ValueError('justifica_texto: argumentos invalidos')
    
    txt = " ".join(txt)

    char = len(txt)
    tup = ()
    res = ()

    if larg > char:
        tup += (txt.ljust(larg),)
        return tup

    while char > larg:
        res = corta_texto_mod(txt, larg)
        tup += (insere_espacos(res[0], larg),)
        txt = res[1]
        char -= len(res[0]) + 1
    
    tup += (res[1].ljust(larg),)
    return tup


'''
Tarefa 2: Calculo de Mandatos pelo método de Hondt
'''

def calcula_quocientes(dic, dep):
    '''
        calcula_quocientes(dic) é a função que recebe um dicionário com os
        votos de um determinado círculo, e devolve a lista por ordem 
        alfabética com o nome de todos os partidos que participaram nas
        eleições.
    '''
    dic2 = dict()
    div = []

    for partido in dic:
        for x in range(1, dep+1):  
            # para cada partido, calcumaos os quocientes dos seus votos
            # até ao número de deputados (range != 0)
            div += [dic[partido] / x,]

        dic2[partido] = div
        div = []

    return dic2


def atribui_mandatos(dic, dep):
    '''
        atribui_mandatos(dic, dep) é a função que recebe um dicionário com os
        votos apurados num círculo e um inteirocrepresentando o número de
        deputados, e devolve a lista ordenada de tamanho igual ao número de
        deputados contendo as cadeias de carateres dos partidos que obtiveram
        cada mandato.
    '''
    lst = []
    lst_quo = []
    dic = calcula_quocientes(dic, dep)

    dic_ord = dict(sorted(dic.items(), key=lambda item: item[1]))

    for partido in dic_ord:
        lst_quo += dic_ord[partido]

    lst_quo = list(set(lst_quo)) # remover quocientes repetidos
    lst_quo.sort(reverse=True)
    lst_quo = lst_quo[:dep]

    for quo in lst_quo:
        for partido in dic_ord:
            for quo2 in dic_ord[partido]:
                if quo == quo2:
                    lst += [partido, ]

    lst = lst[:dep]
    return lst


def obtem_partidos(dic):
    '''
        obtem_partidos(dic) é a função que recebe um dicionário com a informação
        sobre as eleições num território com vários círculos eleitorais como
        descrito, e devolve a lista por ordem alfabética com o nome de todos os
        partidos que participaram nas eleições.
    '''
    nmes = []

    for territorio in dic.values():
        for partido in territorio['votos']:
            if not partido in nmes:
                nmes += [partido, ]

    nmes.sort()
    return nmes


def obtem_resultado_eleicoes(dic):
    '''
        obtem_resultado_eleicoes(dic) é a função que recebe um dicionário com a
        informação sobre as eleições num território com varios círculos
        eleitorais, e devolve a lista ordenada de comprimento igual ao número
        total de partidos com os resultados das eleições.
    '''
    if dic == {} or not isinstance(dic, dict):
        raise ValueError('obtem_resultado_eleicoes: argumento invalido')

    for territorio_name in dic.keys():
        if not isinstance(territorio_name, str):
            raise ValueError('obtem_resultado_eleicoes: argumento invalido')

    for territorio in dic.values():

        if (
            territorio == {} or not isinstance(territorio, dict) or 'deputados'
            not in territorio or 'votos' not in territorio or not isinstance(
            territorio['deputados'], int) or territorio['deputados'] < 1 or 
            len(territorio) != 2 or territorio['votos'] == {} or not 
            isinstance(territorio['votos'], dict)
        ):
            raise ValueError('obtem_resultado_eleicoes: argumento invalido')

        for l_votos in territorio['votos'].keys():
            if not isinstance(l_votos, str):
                raise ValueError('obtem_resultado_eleicoes: argumento invalido')

        for votos in territorio['votos'].values():
            if not isinstance(votos, int) or votos < 1:
                raise ValueError('obtem_resultado_eleicoes: argumento invalido')

    nmes_part = obtem_partidos(dic)
    num_dep = {}
    lst_mandatos = []
    rslt = []

    for territorio in dic.values():
        # criar lista de mandatos totais
        lst_mandatos += atribui_mandatos(territorio['votos'],
        territorio['deputados'])

    for partido in lst_mandatos:
        # criar dicionario com mandatos por partido
        if partido in num_dep:
            num_dep[partido] += 1
        else:
            num_dep[partido] = 1

    for partido in nmes_part:
        vtos = 0
        deps = 0
        for part in num_dep.keys():
            if part == partido:
                deps = num_dep[part]
        for territorio in dic.values():
            for part2 in territorio['votos']:
                if part2 == partido:
                    vtos += territorio['votos'][part2]
        rslt.append((partido, deps, vtos))

    rslt.sort(key=lambda tup: tup[2], reverse=True)
    return rslt


'''
Tarefa 3: Solução dos Sistemas de Equações
'''

def produto_interno(v1, v2):
    '''
        produto_interno(v1, v2) é a função que recebe dois tuplos de números 
        (inteiros ou reais) com a mesma dimensão representando dois vetores e
        retorna o resultado do produto interno desses dois vetores.
    '''
    prod_int = 0
    for x in range(len(v1)):
        prod_int += v1[x] * v2[x]

    return float(prod_int)


def verifica_convergencia(mtrz, const, vetor_var, e):
    '''
        verifica_convergencia(mtrz, vetor_var, const, e) é a função que recebe 
        três tuplos de igual dimensão e um valor real positivo. O primeiro
        tuplo deverá representar uma matriz, o segundo um vetor, e o terceiro
        uma constante, resultado de A * x. A função retornará True caso o erro
        de todas as equações seja inferior ao especificado no valor real e.
    '''
    prod = ()

    for linha in mtrz:
        prod_int_linha = 0
        for cord in range(len(linha)):
            prod_int_linha += linha[cord] * vetor_var[cord]
        prod += (prod_int_linha,)

    for x in range(len(prod)):
        if not abs(prod[x] - const[x]) < e:
            return False
    return True


def retira_zeros_diagonal(mtrz, const):
    '''
        retira_zeros_diagonal(mtrz, const) é a função que recebe um tuplo de 
        tuplos, representando a matriz de entrada, e um tuplo de números, que
        representa o vetor das constantes. A função deverá retornar uma nova
        matriz com as mesmas linhas que a de entrada, mas com estas reordenadas
        de forma a não existirem valores 0 na diagonal. A reorganização será 
        também aplicada no vetor das constantes.
    '''
    mtrz = list(mtrz)
    const = list(const)

    for l in range(len(mtrz)):
        for l2 in range(len(mtrz)):
            if mtrz[l][l] == 0:
                if mtrz[l2][l] != 0 and mtrz[l][l2] != 0:
                    mtrz[l], mtrz[l2] = mtrz[l2], mtrz[l]
                    const[l], const[l2] = const[l2], const[l]

    return tuple(mtrz), tuple(const)


def eh_diagonal_dominante(mtrz):
    '''
        eh_diagonal_dominante(mtrz) é a função que recebe um tuplo de tuplos 
        representando uma matriz quadrada. Deverá retornar True caso seja uma
        matriz diagonalmente dominante, e False caso contrário.
    '''
    for linha in range(len(mtrz)):
        soma_abs = 0
        for n in mtrz[linha]:
            soma_abs += abs(n)
        
        if not abs(mtrz[linha][linha]) >= soma_abs - abs(mtrz[linha][linha]):
            return False

    return True


def resolve_sistema(mtrz, const, e):
    '''
        resolve_sistema(mtrz, const, e) é a função que recebe um tuplo de tuplos 
        representando uma matriz quadrada, um tuplo de números representando o
        valor das constantes e um valor real positivo correspondente à precisão
        pretendida para a solução. Deverá retornar um tuplo que é a solução do
        sistema.
    '''
    if ( 
        not isinstance(mtrz, tuple) or not isinstance(const, tuple) or 
        not isinstance(e, float) or e <= 0 or len(const) != len(mtrz)
    ):
        raise ValueError('resolve_sistema: argumentos invalidos')

    for ln in range(len(mtrz)):
        if not isinstance(mtrz[ln], tuple) or len(mtrz) != len(mtrz[ln]):
            raise ValueError('resolve_sistema: argumentos invalidos')
        for el in range(len(mtrz[ln])):
            if not (isinstance(mtrz[ln][el], int) or isinstance(
            mtrz[ln][el], float)):
                raise ValueError('resolve_sistema: argumentos invalidos')
    
    for el2 in range(len(const)):
        if not (isinstance(const[el2], int) or isinstance(const[el2], float)):
            raise ValueError('resolve_sistema: argumentos invalidos')

    mtrz, const = retira_zeros_diagonal(mtrz, const)
    
    if not eh_diagonal_dominante(mtrz):
        raise ValueError('resolve_sistema: matriz nao diagonal dominante')

    vetor_var = []

    for x in range(len(mtrz)): # criamos vetor
        vetor_var += [0,]

    vetor_var2 = vetor_var[:]
    
    while not verifica_convergencia(mtrz, const, vetor_var, e):
        for l in range(len(mtrz)):
            vetor_var2[l] = vetor_var[l] + (const[l] - (produto_interno(
            mtrz[l], vetor_var))) / mtrz[l][l]
        vetor_var = vetor_var2[:]

    vetor_var = tuple(vetor_var)
    return vetor_var

