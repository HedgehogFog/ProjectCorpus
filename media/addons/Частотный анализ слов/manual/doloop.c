/* ПРИМЕР:
A = 0

DO WHILE A < 0
    A = A + 1
    PRINT A
LOOP
*/

#define DO 228
#define LOOP 229
#define UNTIL 230
#define WHILE 231

struct commands table[16] = {
        ...
		{"do", DO},
		{"loop", LOOP},
		{"until", UNTIL},
		{"while", WHILE},
		...
        {"", END}  /* Маркер конца таблицы */ };
		
int condition();
void doloop_push(char* pointer);
char* doloop_pop();
void doloop_do();
void doloop_loop();

// метод, проверяющий условие, украденный из while.c
int condition() {
    int x, y, cond = 0;
    char op;
    get_exp(&x); /* получить левое выражение */
    get_token(); /* получить оператор */
    if (!strchr("=<>", *token)) {
        serror(0);      /* недопустимый оператор */
        return 0;
    }
    op = *token;
    get_exp(&y);  /* получить правое выражение */
    /* Определение результата */
    switch (op) {
        case '=':
            if (x == y) cond = 1;
            break;
        case '<':
            if (x < y) cond = 1;
            break;
        case '>':
            if (x > y) cond = 1;
            break;
        default:
            break;
    }
    return cond;
}

// DO [WHILE | UNTIL] [CONDITION]
// ...
// ...
// LOOP [WHILE | UNTIL] [CONDITION]

// WHILE = condition, UNTIL = !condition

// Т.к. условие может быть либо в начале цикла, либо в конце, то
// в стек дополнительно записывается 0 - если условие в конце,
// 1 - если условие в начале

char* doloop_stack[FOR_NEST];
int doloop_stack_count = 0;

void doloop_push(char* pointer)
{
	if (doloop_stack_count > FOR_NEST)
	{
		printf("DoLoop Stack Overflow\n");
		exit(228);
	}
	
	doloop_stack[doloop_stack_count] = pointer;
	doloop_stack_count++;
}

char* doloop_pop()
{
	doloop_stack_count--;
	if (doloop_stack_count < 0)
	{
		printf("DoLoop Stack Is Empty\n");
		exit(228);
	}
	
	return doloop_stack[doloop_stack_count];
}

void doloop_do()
{
	// Сохраняем указатель на место перед командой DO
	putback();
	char* do_pointer = prog;
	get_token();
	
	// Проверяем место условия, если оно не в начале, то оно в конце
	get_token();
	if (tok == WHILE || tok == UNTIL)
	{
		// Вычисляем условие, если UNTIL, то инвентируем
		int result = (tok == WHILE ? condition() : !condition());
		
		// Проверяем условие
		if (result)
		{
			doloop_push(do_pointer); // Добавляем указатель на начало в стек
			doloop_push((char*)1); // Добавляем информацию о том, что условие в начале, в стек
			return;
		}
		else // Условие не выполнено
		{
			// Скипаем все команды до LOOP, т.к. условие не выполнилось
			while (tok != LOOP)
				get_token();
			
			get_token();
			return;
		}
	}
	else // Если условие в конце
	{
		doloop_push(do_pointer); // Добавляем указатель на начало в стек
		doloop_push((char*)0); // Добавляем информацию о том, что условие в конце, в стек
		return;
	}
}

void doloop_loop()
{
	char* condition_in_do = doloop_pop(); // Получаем из стека место условия
	char* pointer_to_do = doloop_pop(); // Получаем из стека указатель на начало цикла
	
	if (condition_in_do) // Если условие в начале (в DO, а не в LOOP)
	{
		prog = pointer_to_do; // Перемещаемся на начало цикла
		return;
	}
	else // Условие в конце
	{
		get_token();
		
		// Т.к. у нас есть информация, что условие в конце, токен обязательно
		// должен быть равен WHILE или UNTIL
		if (tok != WHILE && tok != UNTIL)
		{
			printf("DoLoop Wrong Construction\n");
			exit(228);
		}
		
		// Вычисляем условие, если UNTIL, то инвентируем
		int result = (tok == WHILE ? condition() : !condition());
		
		// Проверяем условие
		if (result)
		{
			prog = pointer_to_do; // Перемещаемся на начало цикла
			return;
		}
		else // Если условие не выполнено
		{
			// Ничего не делаем, интерпретатор будет интерпретировать следующие команды
			// для интерпретации, которые находятся после цикла
			return;
		}
	}
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
				case DO:
					doloop_do();
					break;
				case LOOP:
					doloop_loop();
					break;
				..
                case END:
                    exit(0);
                default:
                    break;
            }
    } while (tok != FINISHED);

}

// конский хуй