// Основная идея: 
// вынести результат условия из конструкции IF,
// когда анализатор встретит ELSE он обратится
// к последнему условию, и если оно истинно 
// пропустит весь ELSE, а иначе выполнит все в строке

#define ELSE 20

/* Команда должна вводится прописными буквами в эту таблицу */
struct commands table[13] = {
        ...
        {"else", ELSE},
        {"", END}  /* Маркер конца таблицы */
};

/* Условие для IF теперь глобальная переменная */
int if_cond = 0;

/* IF */
void exec_if() {
    int x, y;
    char op;
    get_exp(&x); /* получить левое выражение */
    get_token(); /* получить оператор */
    if (!strchr("=<>", *token)) {
        serror(0);      /* недопустимый оператор */
        return;
    }
    op = *token;
    get_exp(&y);  /* получить правое выражение */
    /* Определение результата */
    if_cond = 0;
    switch (op) {
        case '=':
            if (x == y) if_cond = 1;
            break;
        case '<':
            if (x < y) if_cond = 1;
            break;
        case '>':
            if (x > y) if_cond = 1;
            break;
        default:
            break;
    }

    if (if_cond) {  /* если значение IF "истина"  */
        get_token();
        if (tok != THEN) {
            serror(8);
            return;
        } /* иначе, программа выполняется со следующей строки */
    } else find_eol(); /* поиск точки старта программы */

}

/* ELSE */
void exec_else() {
	/* если условие истинно, значит, нужно пропустить весь ELSE */
    if(if_cond) { find_eol(); }
    else{ /* ничего, продолжаем программу сразу после ELSE */ }
}

int main(int argc, char *argv[]) {
    ...
    do {
        token_type = get_token();
        /* проверка на оператор присваивания */
        if (token_type == VARIABLE) {
            putback(); /* возврат пер. обратно во входной поток */
            assignment(); /* должен быть оператор присваивания */
        } else /* это команда */
            switch (tok) {
                ...
                case ELSE:
                    exec_else();
                    break;
                ...
            }
    } while (tok != FINISHED);
}