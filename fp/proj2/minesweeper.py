'''
1 - TAD gerador
'''

##
# CONSTRUTORES
##

def cria_gerador(b, s):
    '''
    número de bits do gerador (int) x seed inicial (int) -> gerador (g)
    '''
    if not (isinstance(b, int) and isinstance(s, int)) or b not in (32, 64) or s < 1:
        raise ValueError('cria_gerador: argumentos invalidos')
    if (b == 32 and s > (2**32)) or (b == 64 and s > (2**64)):
        raise ValueError('cria_gerador: argumentos invalidos')

    return [b, s]


def cria_copia_gerador(g):
    '''
    gerador (g) -> cópia de gerador (g2)
    '''
    return [g[0], g[1]]


##
# SELETOR
##

def obtem_estado(g):
    '''
    gerador (g) -> estado (int)
    '''
    return[g[1]]


##
# MODIFICADORES
##

def define_estado(g, s):
    '''
    gerador (g) x seed inicial (int) -> seed inicial (int)
    '''
    g = [g[0], s]
    return s


def atualiza_estado(g):
    '''
    gerador (g) -> seed modificado (int)
    '''
    if g[0] == 32:
        g[1] ^= (g[1] << 13) & 0xFFFFFFFF
        g[1] ^= (g[1] >> 17) & 0xFFFFFFFF
        g[1] ^= (g[1] << 5) & 0xFFFFFFFF

    if g[0] == 64:
        g[1] ^= (g[1] << 13) & 0xFFFFFFFFFFFFFFFF
        g[1] ^= (g[1] >> 7) & 0xFFFFFFFFFFFFFFFF
        g[1] ^= (g[1] << 17) & 0xFFFFFFFFFFFFFFFF
        
    g = [g[0], g[1]]

    return g[1]


##
# RECONHECEDOR
##

def eh_gerador(g):
    '''
    gerador (g) -> booleano (True se for gerador)
    ''' 
    if not isinstance(g, list):
        return False
    if len(g) != 2:
        return False
    if g[0] not in (32, 64) or g[1] < 1:
        return False
    return True


##
# TESTE
##

def geradores_iguais(g1, g2):
    '''
    gerador (g1) x gerador (g2) -> igualdade (bool)
    '''
    if g1[0] != g2[0] or g1[1] != g2[1]:
        return False
    return True


##
# TRANSFORMADOR
##

def gerador_para_str(g):
    '''
    número de bits do gerador (int) x seed inicial (int) -> gerador (str)
    '''
    return str(f'xorshift{g[0]}(s={g[1]})')


##
# FUNÇÕES DE ALTO NÍVEL
##

def gera_numero_aleatorio(g, n):
    '''
    gerador inicial (g) x novo seed (int) -> número aleatório (int)
    '''
    return (1 + (atualiza_estado(g) % n))


def gera_carater_aleatorio(g, c):
    '''
    gerador inicial (g) x caracter (str) -> caracter aleatório (str)
    '''
    # (1 + ...) para que conte todos os números entre 'A' e c, e não exclua
    # um deles
    return chr(atualiza_estado(g) % (1 + ord(c) - ord('A')) + ord('A'))


'''
2 - TAD coordenada
'''

##
# CONSTRUTORES
##

def cria_coordenada(col, lin):
    '''
    coluna (str) x linha (int) -> coordenada (cord)
    '''
    if not (isinstance(col, str) and isinstance(lin, int)):
        raise ValueError('cria_coordenada: argumentos invalidos')

    if len(col) != 1:
        raise ValueError('cria_coordenada: argumentos invalidos')

    if not (ord('A') <= ord(col) <= ord('Z')):
        raise ValueError('cria_coordenada: argumentos invalidos')
    
    if not (1 <= lin <= 99):
        raise ValueError('cria_coordenada: argumentos invalidos')

    return (col, lin)


##
# SELETOR
##

def obtem_coluna(cord):
    '''
    coordenada (cord) -> coluna (str)
    '''
    return cord[0]


def obtem_linha(cord):
    '''
    coordenada (cord) -> linha (int)
    '''
    return cord[1]


##
# RECONHECEDOR
##

def eh_coordenada(arg):
    '''
    argumento (arg) -> igualdade (bool)
    (caso o argumento seja uma coordenada, retorna True)
    '''
    if not isinstance(arg, tuple):
        return False
    if not (isinstance(arg[0], str) and 
    isinstance(arg[1], int) and len(arg) == 2
    ):
        return False
    if len(arg[0]) != 1:
        return False
    if not (ord('A') <= ord(arg[0]) <= ord('Z')):
        return False
    if not (1 <= arg[1] <= 99):
        return False
    return True


##
# TESTE
##

def coordenadas_iguais(cord1, cord2):
    '''
    coordenada 1 (cord) x coordenada 2 (cord) -> igualdade (bool)
    (caso sejam iguais, a função retorna True)
    '''
    if cord1[0] == cord2[0] and cord1[1] == cord2[1]:
        return True
    return False


##
# TRANSFORMADORES
##

def coordenada_para_str(cord):
    '''
    coordenada (cord) -> string (str)
    '''
    if cord[1] < 10:
        return f'{cord[0]}0{cord[1]}'
    else:
        return f'{cord[0]}{cord[1]}'


def str_para_coordenada(s):
    '''
    string (str) -> coordenada (cord)
    '''
    if len(s) == 3:
        col = s[0]
        lin = int(s[1]+s[2])
        return cria_coordenada(col, lin)
    return

    
##
# FUNÇÕES DE ALTO NÍVEL
##

def obtem_coordenadas_vizinhas(cord):
    '''
    coordenadas (cord) -> tuplo com as coordenadas vizinhas (tup)
    '''
    vzinh = []
    col = obtem_coluna(cord)
    lin = obtem_linha(cord)

    if lin > 1 and ord(col) > ord('A'):
        # caso a coordenada não esteja em nenhuma das 2 pontas
        vzinh.append(cria_coordenada(chr(ord(col) - 1), lin - 1))

    if lin > 1:
        vzinh.append(cria_coordenada(col, lin - 1))

    if lin > 1 and ord(col) < ord('Z'):
        vzinh.append(cria_coordenada(chr(ord(col) + 1), lin - 1))
    
    if ord(col) < ord('Z'):
        vzinh.append(cria_coordenada(chr(ord(col) + 1), lin),)

    if lin < 99 and ord(col) < ord('Z'):
        vzinh.append(cria_coordenada(chr(ord(col) + 1), lin + 1))

    if lin < 99:
        vzinh.append(cria_coordenada(col, lin + 1))

    if ord(col) > ord('A') and lin < 99:
        vzinh.append(cria_coordenada(chr(ord(col) - 1), lin + 1))
    
    if ord(col) > ord('A'):
        vzinh.append(cria_coordenada(chr(ord(col) - 1), lin))
    
    return tuple(vzinh)


def obtem_coordenada_aleatoria(cord, g):
    '''
    coordenadas (cord) x gerador (g) -> coordenadas aleatórias (g)
    '''
    col = gera_carater_aleatorio(g, obtem_coluna(cord))
    lin = gera_numero_aleatorio(g, obtem_linha(cord))

    return cria_coordenada(col, lin)


'''
3 - TAD parcela
'''

##
# CONSTRUTORES
##

def cria_parcela():
    '''
    {} -> parcela tapada (p)
    '''
    return ['tapada', 0]

def cria_copia_parcela(p1):
    '''
    parcela (p1) -> cópia de parcela (p2)
    '''
    return [p1[0], p1[1]]


##
# MODIFICADORES
##

def limpa_parcela(p):
    '''
    parcela (p) -> parcela limpa (p)
    '''
    p[0] = 'limpa'
    return p


def marca_parcela(p):
    '''
    parcela (p) -> parcela marcada (p)
    '''
    p[0] = 'marcada'
    return p


def desmarca_parcela(p):
    '''
    parcela (p) -> parcela tapada (p)
    '''
    p[0] = 'tapada'
    return p

def esconde_mina(p):
    '''
    parcela (p) -> parcela com mina (p)
    '''
    p[1] = 1
    return p


##
# RECONHECEDOR
##

def eh_parcela(arg):
    '''
    argumento (arg) -> igualdade (bool)
    (caso o argumento seja uma parcela, retorna True)
    '''
    if not isinstance(arg, list):
        return False
    if not len(arg) == 2:
        return False
    if not (isinstance(arg[0], str) and isinstance(arg[1], int)):
        return False
    return True


def eh_parcela_tapada(p):
    '''
    parcela (p) -> igualdade (bool)
    (caso o argumento seja uma parcela tapada, retorna True)
    '''
    if p[0] == 'tapada':
        return True

    return False


def eh_parcela_marcada(p):
    '''
    parcela (p) -> igualdade (bool)
    (caso o argumento seja uma parcela marcada, retorna True)
    '''
    if p[0] == 'marcada':
        return True

    return False

def eh_parcela_limpa(p):
    '''
    parcela (p) -> igualdade (bool)
    (caso o argumento seja uma parcela limpa, retorna True)
    '''
    if p[0] == 'limpa':
        return True

    return False


def eh_parcela_minada(p):
    '''
    parcela (p) -> igualdade (bool)
    (caso o argumento seja uma parcela minada, retorna True)
    '''
    if p[1] == 1:
        return True

    return False


##
# TESTE
##

def parcelas_iguais(p1, p2):
    '''
    parcela 1 (p1) x parcela 2 (p2) -> igualdade (bool)
    (caso sejam iguais, a função retorna True)
    '''
    if p1[0] == p2[0] and p1[1] == p2[1]:
        return True

    return False


##
# TRANSFORMADOR
##

def parcela_para_str(p):
    '''
    parcela (p) -> parcela (str)
    '''
    if eh_parcela_tapada(p):
        return '#'
    if eh_parcela_marcada(p):
        return '@'
    if eh_parcela_limpa(p) and eh_parcela_minada(p):
        return 'X'
    if eh_parcela_limpa(p) and not eh_parcela_minada(p):
        return '?'


##
# FUNÇÕES DE ALTO NÍVEL
##

def alterna_bandeira(p):
    '''
    parcela (p) -> parcela alterada ou não (bool)
    (caso existam alterações, retorna True)
    '''
    if eh_parcela_marcada(p):
        p = desmarca_parcela(p)
        return True
    if eh_parcela_tapada(p):
        p = marca_parcela(p)
        return True
    return False


'''
4 - TAD campo
'''

##
# CONSTRUTORES
##

def cria_campo(c, l):
    '''
    coluna final (str) x linha final (int) -> campo (m)
    '''
    if not (isinstance(c, str) and isinstance(l, int)):
        return ValueError('cria_campo: argumentos invalidos')
    if len(c) != 1 or not (0 < l < 99):
        return ValueError('cria_campo: argumentos invalidos')
    if ord(c) > ord('Z'):
        return ValueError('cria_campo: argumentos invalidos')
    
    m = {}
    for lin in range(1, l + 1):
        # para cada coluna no intervalo de letras
        for col in range(ord('A'), ord(c) + 1):
            m[cria_coordenada(chr(col), lin)] = cria_parcela()
    return m


def cria_copia_campo(m):
    '''
    campo (m) -> cópia do campo (m2)
    '''
    m2 = m.copy()
    return m2


##
# SELETOR
##

def obtem_ultima_coluna(m):
    '''
    campo (m) -> última coluna (str)
    '''
    cord_list = [cord for cord in m.keys()]
    return cord_list[-1][0]


def obtem_ultima_linha(m):
    '''
    campo (m) -> última linha (int)
    '''
    cord_list = [cord for cord in m.keys()]
    return cord_list[-1][1]


def obtem_parcela(m, cord):
    '''
    campo (m) x coordenada (cord) -> parcela (p)
    '''
    return m[cord]


def obtem_coordenadas(m, s):
    '''
    campo (m) x estado (s) -> lista de coordenadas nesse estado (lst)
    '''
    lst_cord = []
    for cord in m.keys():
        if s == 'minadas':
            if eh_parcela_minada(m[cord]):
                # quando a função for minada, retorna-a instantaneamente
                lst_cord.append(cord)
        elif m[cord][0] == s[:-1]:
            # s[:-1] para tirar a última letra: "limpas" -> "limpa"
            lst_cord.append(cord)
            
    return tuple(lst_cord)

def obtem_numero_minas_vizinhas(m, cord):
    '''
    campo (m) x coordenada (cord) -> número de minas vizinhas (int)
    '''
    cord_viz = list(obtem_coordenadas_vizinhas(cord))
    for cord in cord_viz:
        if cord not in m.keys():
            cord_viz.remove(cord)
    minas_vz = 0
    for c in cord_viz:
        if c in m.keys() and eh_parcela_minada(m[c]):
            minas_vz += 1

    return minas_vz


##
# RECONHECEDOR
##

def eh_campo(arg):
    '''
    argumento (arg) -> igualdade (bool)
    (caso o argumento seja um campo, retorna True)
    '''
    if not isinstance(arg, dict):
        return False

    for cord in arg.keys():
        if not eh_coordenada(cord):
            return False

    for parc in arg.values():
        if not eh_parcela(parc):
            return False

    return True


def eh_coordenada_do_campo(m, cord):
    '''
    campo (m) x coordenada (cord) -> igualdade (bool)
    (caso a coordenada esteja no campo, retorna True)
    '''
    if cord not in m.keys():
        return False
    
    return True


##
# TESTE
##

def campos_iguais(m1, m2):
    '''
    campo 1 (m) x campo 2 (m) -> igualdade (bool)
    (caso a coordenada esteja no campo, retorna True)
    '''
    for cord in m1.keys():
        if cord not in m2.keys():
            return False
    
    return True


##
# TRANSFORMADOR
##

def campo_para_str(m):
    '''
    campo (m) -> campo (str)
    '''
    campo_str = ''

    # linha 1
    col_list = ''
    lin_list = ()
    for cord in m.keys():
        if obtem_coluna(cord) not in col_list:
            # a col_list serve para listar as nossas colunas na interface
            col_list += obtem_coluna(cord)

    campo_str += '   ' + col_list

    # linha 2
    campo_str += '\n'
    campo_str += '  ' + '+' + ('-' * len(col_list)) + '+'

    # linhas restantes
    for cord in m.keys():
        if obtem_linha(cord) not in lin_list:
            lin_list += (obtem_linha(cord),)

    for lin in range(1, len(lin_list) + 1):
        campo_str += '\n'
        lin_str = []
        for parc in m.keys():
            if obtem_linha(parc) == lin:
                if eh_parcela_limpa(m[parc]):
                    lin_str += str(obtem_numero_minas_vizinhas(m, parc))
                elif not eh_parcela_limpa(m[parc]):
                    lin_str += parcela_para_str(m[parc])

        lin_str = [' ' if x=='0' else x for x in lin_str] # remover 0's

        if len(str(lin)) == 1:
            # se 0 < lin < 10, temos que colocar um 0 atrás de lin
            campo_str += f'0{lin}' + '|' + ''.join(lin_str) + '|'
        elif len(str(lin)) == 2:
            campo_str += str(lin) + '|' + ''.join(lin_str) + '|'
    
    # última linha
    campo_str += '\n'
    campo_str += '  ' + '+' + ('-' * len(col_list)) + '+'

    return campo_str


##
# FUNÇÕES DE ALTO NÍVEL
##

def coloca_minas(m, cord, g, n):
    '''
    campo (m) -> campo (str)
    '''
    n_mina_col = 0
    ult_cord = cria_coordenada(obtem_ultima_coluna(m), obtem_ultima_linha(m))
    cord_viz = obtem_coordenadas_vizinhas(cord)
    while n_mina_col < n:
    # enquanto não tivermos o número de minas pedido
        mina_cord = obtem_coordenada_aleatoria(ult_cord, g)
        if mina_cord != cord and mina_cord not in cord_viz and not eh_parcela_minada(obtem_parcela(m, mina_cord)):
            n_mina_col += 1
            esconde_mina(obtem_parcela(m, mina_cord))

    return m


def limpa_campo(m, cord):
    '''
    campo (m) x coordenada (cord) -> campo limpo (m)
    '''
    por_limpar = []
    limpas = []
    # limpas serão as coordenadas já limpas ao longo do nosso código.
    # as por_limpar irão conter as coordenadas tapadas sem minas vizinhas.

    if eh_parcela_limpa(obtem_parcela(m, cord)):
        return m

    if obtem_numero_minas_vizinhas(m, cord) == 0:
        cv = obtem_coordenadas_vizinhas(cord)
        for c in cv:
            if (eh_coordenada_do_campo(m, c) and 
            eh_parcela_tapada(obtem_parcela(m, c))
            ):
                por_limpar += (c,)
                limpas = cord

    while len(por_limpar) > 0:
        for n in por_limpar:
            if obtem_numero_minas_vizinhas(m, n) == 0:
                limpa_parcela(obtem_parcela(m, n))
                limpas += (n,)

                for z in obtem_coordenadas_vizinhas(n):
                    if (eh_coordenada_do_campo(m, z) 
                    and eh_parcela_tapada(obtem_parcela(m, z))
                    and z not in por_limpar
                    and z not in limpas
                    ):
                        limpa_parcela(obtem_parcela(m, z))
                        por_limpar += (z,)

            por_limpar = por_limpar[1:] # removemos a limpa

    if obtem_numero_minas_vizinhas != 0:
        limpa_parcela(obtem_parcela(m, cord))
        
    return m


'''
Funções adicionais
'''

def jogo_ganho(m):
    '''
    campo (m) -> igualdade (bool)
    (retorna True se o jogo estiver ganho)
    '''
    for cord in m:
        if not (eh_parcela_minada(obtem_parcela(m, cord)) or eh_parcela_limpa(obtem_parcela(m, cord))):
            return False
    return True


def turno_jogador(m):
    '''
    campo (m) -> igualdade (bool)
    '''
    a = 0
    cord = ''

    while not (a == 'L' or a == 'M'):
        a = str(input('Escolha uma ação, [L]impar ou [M]arcar:'))
    
    while not eh_coordenada_do_campo(m, cord):
        cord = str_para_coordenada(str(input('Escolha uma coordenada:')))

    if a == 'L':
        if eh_parcela_minada(obtem_parcela(m, cord)):
            return False
        else:
            limpa_campo(m, cord)
            return True
    
    if a == 'M':
        alterna_bandeira(obtem_parcela(m, cord))
        return True


def bandeiras(m):
    '''
    campo (m) -> número de bandeiras (int)
    '''
    n_band = 0
    for cord in m:
        if eh_parcela_marcada(obtem_parcela(m, cord)):
            n_band += 1
    
    return n_band

def primeiro_turno(m, g, n):
    '''
    campo (m) -> igualdade (bool)
    '''
    cord = ''

    while not eh_coordenada_do_campo(m, cord):
        cord = str_para_coordenada(str(input('Escolha uma coordenada:')))

    coloca_minas(m, cord, g, n)

    if eh_parcela_minada(obtem_parcela(m, cord)):
        return False
    else:
        limpa_campo(m, cord)
        return True

def minas(c, l, n, d, s):
    '''
    última coluna (c) x última linha (l) x número de parcelas com minas (n)
    x dimensão do gerador (g) x estado inicial (s) -> igualdade (bool)
    (retorna True em caso de vitória)
    '''
    if not (isinstance(c, str) and isinstance(l, int) and isinstance(n, int)
    and isinstance(d, int) and isinstance(s, int) and d in (32, 64)
    ):
        raise ValueError('minas: argumentos invalidos')
    if not (ord('A') <= ord(c) <= ord('Z')):
        raise ValueError('minas: argumentos invalidos')

    m = cria_campo(c, l)
    g = cria_gerador(d, s)
    print(f'   [Bandeiras: {bandeiras(m)}/{n}]')
    print(campo_para_str(m))

    primeiro_turno(m, g, n)

    while not jogo_ganho(m):
        print(f'   [Bandeiras: {bandeiras(m)}/{n}]')
        print(campo_para_str(m))
        result = turno_jogador(m)

        if not result:
            print('BOOOOOOOM!!!')
            return False
    
    print(f'   [Bandeiras: {bandeiras(m)}/{n}]')
    print(campo_para_str(m))
    print('VITORIA!!!')
    return False

minas('Z', 25, 30, 64, 2)