const Code = document.getElementById('code');
const Terminal = document.getElementById('terminal');

let varsTable = {};

function parseListArray(list) {
    return list.replace(/\s+/g, '').slice(1, -1).split(',');
}

function Print(text, scoope = 0) {
    if (!text.str) {
        if (typeof text.w === 'string' && varsTable[text.w]) {
            if (varsTable[text.w].scoope > scoope) {
                Terminal.innerText += `SeholRuntimeError: The variable "${text.w}" was undefined.`;
                throw new Error(`SeholRuntimeError: The variable "${text.w}" was undefined.`);
            }
            Terminal.innerText += varsTable[text.w].v;
        } else if (varsTable[text.w[0]]?.type === "ListArray" || varsTable[text.w[0]]?.type === "String") {
            if (varsTable[text.w[0]].scoope > scoope) {
                Terminal.innerText += `SeholRuntimeError: The variable "${text.w}" was undefined.`;
                throw new Error(`SeholRuntimeError: The variable "${text.w}" was undefined.`);
            }
            const array = varsTable[text.w[0]].v;
            if (array.length <= text.w[1]) {
                const errorMsg = `SeholRuntimeError: The index [${text.w[1]}] of array ${text.w[0]} is out of range.`;
                Terminal.innerText += errorMsg;
                throw new Error(errorMsg);
            }
            Terminal.innerText += array[text.w[1]].replace(/"/g, '');
        } else {
            Terminal.innerText += JSON.stringify(text.w);
        }
    } else {
        Terminal.innerText += text.w === " " ? "\u00A0" : text.w;
    }
}

let isAnString = false;
function Lexer(input) {
    const tokens = [];
    const tokenTypes = [
        { regex: /^\s+/, type: 'Blank' },
        { regex: /^\d+/, type: 'Number' },
        { regex: /^[a-zA-Z_]\w*/, type: 'Identifier' },
        { regex: /^,/, type: 'Comma' },
        { regex: /^:/, type: 'Collon' },
        { regex: /^==/, type: 'eqOp' },
        { regex: /^!=/, type: 'neqOp' },
        { regex: /^</, type: 'mnqOp' },
        { regex: /^>/, type: 'mqOp' },
        { regex: /^&/, type: 'andOp' },
        { regex: /^\|/, type: 'orOp' },
        { regex: /^!/, type: 'notOp' },
        { regex: /^=/, type: 'assignOp' },
        { regex: /^\+\+/, type: 'incOp' },
        { regex: /^\+/, type: 'plusOp' },
        { regex: /^--/, type: 'descOp' },
        { regex: /^-/, type: 'minusOp' },
        { regex: /^\*/, type: 'timesOp' },
        { regex: /^\//, type: 'divOp' },
        { regex: /^\(/, type: 'openParen' },
        { regex: /^\)/, type: 'closeParen' },
        { regex: /^\{/, type: 'openKeys' },
        { regex: /^\}/, type: 'closeKeys' },
        { regex: /^\[/, type: 'openBrack' },
        { regex: /^]/, type: 'closeBrack' },
        { regex: /^\"/, type: 'quote' },
        { regex: /^;/, type: 'semicolon' },
    ];

    let remainingInput = input;
    let currentString = '';

    while (remainingInput.length > 0) {
        let matched = false;

        for (const { regex, type } of tokenTypes) {
            const match = regex.exec(remainingInput);
            if (match) {
                if (type) {
                    if (type === 'quote') {
                        isAnString = !isAnString;
                        if (!isAnString) {
                            tokens.push({ name: 'String', token: currentString });
                            currentString = '';
                        }
                    } else if (isAnString) {
                        currentString += match[0];
                    } else if (type !== 'Blank') {
                        tokens.push({ name: type, token: match[0] });
                    }
                }
                remainingInput = remainingInput.slice(match[0].length);
                matched = true;
                break;
            }
        }
        if (!matched) {
            if (isAnString) {
                currentString += remainingInput[0];
                remainingInput = remainingInput.slice(1);
            } else {
                throw new Error(`Unexpected token: ${remainingInput[0]}`);
            }
        }
    }

    tokens.push({ name: 'EOF', token: '' });
    return tokens;
}

function typeThis(t) {
    const tokenTypes = [
        { regex: /^\d+/, type: 'Number' },
        { regex: /^(true|false)$/, type: 'Boolean' },
        { regex: /^\[([^\]]*)\]$/, type: 'ListArray' },
        { regex: /^[a-zA-Z_]\w*/, type: 'String' }
    ]
    for (const { regex, type } of tokenTypes) {
        const match = regex.exec(t);
        if (match) {
            if (type) {
                return type;
            } else if (t.call) {
                return 'nativeFn';
            }
        }
    }
}

function Parser(tree) {
    let index = 0;
    let currentToken = tree[index];

    function advance() {
        if (tree.length > index) {
            index++;
            currentToken = tree[index];
        } else {
            throw new Error("Sehol: Advanc exceed bounds.");
        }
    }

    function expect(t) {
        if (t != currentToken.token) {
            Print({ w: "SeholSyntaxError: Expect \"" + t + "\", but we got " + currentToken.token, str: true });
            throw new Error("SeholSyntaxError: Expect \"" + t + "\", but we got " + currentToken.token);
        }
    }

    function factor() {
        let token = currentToken;
        let index;
        if (token.name === 'Number' || token.name === 'Identifier' || token.name === 'String') {
            advance();
            if (currentToken.token == "[") {
                advance();
                index = expression();
            }
            return { token, index };
        } else if (token.name === 'openParen') {
            advance();
            let result = expression();
            if (currentToken.name === 'closeParen') {
                advance();
                return result;
            }
        }
    }

    function term() {
        let result = factor();
        while (currentToken.name == 'timesOp' || currentToken.name == 'divOp') {
            let token = currentToken;
            if (token.name === 'timesOp') {
                advance();
                result = {
                    name: 'expression',
                    op: 'timesOp',
                    left: result,
                    right: factor()
                };
            } else if (token.name === 'divOp') {
                advance();
                result = {
                    name: 'expression',
                    op: 'divOp',
                    left: result,
                    right: factor()
                };
            }
        }
        return result;
    }

    function logicExpression() {
        let token = tree[index + 1];
        let result = expression();
        while (token.name === 'eqOp' || token.name === 'neqOp' || token.name === 'andOp' || token.name === 'orOp' || token.name === 'notOp' || token.name === 'mqOp' || token.name === 'mnqOp') {
            if (token.name === 'eqOp') {
                result = {
                    name: 'logicExpression',
                    op: 'eqOp',
                    left: result,
                    right: expression()
                };
                token = currentToken;
            } else if (token.name === 'neqOp') {
                result = {
                    name: 'logicExpression',
                    op: 'neqOp',
                    left: result,
                    right: expression()
                };
                token = currentToken;
            } else if (token.name === 'mqOp') {
                result = {
                    name: 'logicExpression',
                    op: 'mqOp',
                    left: result,
                    right: expression()
                };
                token = currentToken;
            } else if (token.name === 'mnqOp') {
                result = {
                    name: 'logicExpression',
                    op: 'mnqOp',
                    left: result,
                    right: expression()
                };
                token = currentToken;
            } else if (token.name === 'andOp') {
                result = {
                    name: 'logicExpression',
                    op: 'andOp',
                    left: result,
                    right: expression()
                };
                token = currentToken;
            } else if (token.name === 'orOp') {
                result = {
                    name: 'logicExpression',
                    op: 'orOp',
                    left: result,
                    right: expression()
                };
                token = currentToken;
            } else if (token.name === 'notOp') {
                result = {
                    name: 'logicExpression',
                    op: 'notOp',
                    left: result
                };
                token = currentToken;
            }
        }
        return result;
    }

    function expression() {
        let result = term();
        while (currentToken.name === 'plusOp' || currentToken.name === 'minusOp') {
            let token = currentToken;
            if (token.name === 'plusOp') {
                advance();
                result = {
                    name: 'expression',
                    op: 'plusOp',
                    left: result,
                    right: term()
                };
            } else if (token.name === 'minusOp') {
                advance();
                result = {
                    name: 'expression',
                    op: 'minusOp',
                    left: result,
                    right: term()
                };
            }
        }
        advance();
        return result;
    }

    function parseIf() {
        advance();
        advance();
        let condition = logicExpression();
        expect(")");
        advance();
        expect("{")
        advance();
        while (currentToken.token != '}') {
            if (currentToken.name === 'EOF') {
                Print({ w: "SeholSyntaxError: The while loop is missing an close key. \"}\"", str: true });
                throw new Error("SeholSyntaxError: The while loop is missing an close key. \"}\"");
            }
            consequent.push(statement());
        }
        expect("}");
        let elseCondition;
        if (tree[index + 1].token === "else") {
            advance();
            advance();
            expect("{");
            advance();
            elseCondition = statement();
            expect("}");
        }
        return {
            name: 'ifStatement',
            condition,
            consequent,
            elseCondition
        }
    }

    function parseVar() {
        advance();
        const id = currentToken.token;
        advance();
        let type;
        if (currentToken.token === ":") {
            advance();
            type = currentToken.token;
            advance();
        }
        expect("=");
        advance();
        let value = '';
        if (currentToken.token == "[") {
            while (currentToken.token != "]") {
                if (currentToken.name == "EOF") {
                    throw new Error("SeholSyntaxError: You miss an \"]\".");
                }
                value += currentToken.token;
                advance();
            }
            value += ']';
            advance();
            expect(";");
            advance();
        } else if (currentToken.token == "-") {
            value += currentToken.token;
            advance();
            value += currentToken.token;
            advance();
            expect(";");
            advance();
        } else {
            value = statement().token;
            if (currentToken.token == ";") {
                advance();
            }
        }
        value = value?.token ? value.token : value
        const valueType = typeThis(value);
        if (valueType == "ListArray") {
            value = parseListArray(value);
        }
        if (type && valueType != type) {
            const errorMsg = `SeholRuntimeError: The attribution of type \"${valueType}\" is different from the type of variable \"${id}\" that is \"${type}\".`;
            Print({ w: errorMsg, str: true });
            throw new Error(errorMsg);
        }
        return {
            name: 'varDeclaration',
            id,
            type: valueType,
            value
        }
    }

    function parseFn() {
        let call = currentToken.token;
        advance();
        expect("(");
        advance();
        let args = [];
        let indexArray = '';
        while (currentToken.token != ')') {
            if (currentToken.token != ',' && currentToken.token != '[' && currentToken.token != ']') {
                if (tree[index + 1].token != "," && tree[index + 1].token != ")" && tree[index + 1].token != "[" && tree[index + 1].token != "]") {
                    const message = "SeholSyntaxError: After \"" + currentToken.token + "\", is expected \",\", \")\", or \"[\", but is finded \"" + tree[index + 1].token + "\".";
                    Print({ w: message, str: true });
                    throw new Error(message);
                }
                if (tree[index + 1].token != "[") {
                    args.push({ w: currentToken.token, str: currentToken.name === "String" });
                } else {
                    const id = currentToken.token;
                    advance();
                    if (currentToken.token == "[") {
                        while (currentToken.token != "]") {
                            advance();
                            if (currentToken.name == "EOF") {
                                throw new Error("SeholSyntaxError: You miss an \"]\".");
                            }
                            indexArray += currentToken.token;
                        }
                    }
                    args.push({ w: [id, indexArray.slice(0, -1)], str: currentToken.name === "String" });
                }
            }
            advance();
        }
        advance()
        expect(";");
        advance();
        return {
            name: "fnCall",
            call,
            args
        }
    }

    function parseChangeVar() {
        const id = currentToken.token;
        advance();
        expect("=");
        advance();
        let value = '';
        if (currentToken.token == "[") {
            value = currentToken.token;
            while (currentToken.token != "]") {
                if (currentToken.name == "EOF") {
                    throw new Error("SeholSyntaxError: You miss an \"]\".");
                }
                value += currentToken.token;
                advance();
            }
            value += ']';
            value = parseListArray(value);
            expect(";");
            advance();
        } else if (currentToken.token == "-") {
            value += currentToken.token;
            advance();
            value += currentToken.token;
            advance();
            expect(";");
            advance();
        } else {
            value = statement();
            if (currentToken.token == ";") {
                advance();
            }
        }
        value = value?.token ? value.token : value
        return {
            name: "changeVarValue",
            id,
            value
        }
    }

    function parseChangeVarByIndex() {
        const id = currentToken.token;
        advance();
        expect("[");
        advance();
        const indexArray = expression();
        advance();
        let value = '';
        if (currentToken.token == "[") {
            value = currentToken.token;
            while (currentToken.token != "]") {
                if (currentToken.name == "EOF") {
                    throw new Error("SeholSyntaxError: You miss an \"]\".");
                }
                value += currentToken.token;
                advance();
            }
            value += ']';
            value = parseListArray(value);
            expect(";");
            advance();
        } else if (currentToken.token == "-") {
            value += currentToken.token;
            advance();
            value += currentToken.token;
            advance();
            expect(";");
            advance();
        } else {
            value = expression();
            if (currentToken.token == ";") {
                advance();
            }
        }
        value = value?.token ? value.token : value
        return {
            name: "changeVarValue",
            id,
            index: indexArray,
            value
        }
    }

    function parseWhile() {
        advance();
        expect("(");
        advance();
        const condition = logicExpression();
        expect(")");
        advance();
        expect("{");
        advance();
        let consequent = [];
        while (currentToken.token != '}') {
            if (currentToken.name === 'EOF') {
                Print({ w: "SeholSyntaxError: The while loop is missing an close key. \"}\"", str: true });
                throw new Error("SeholSyntaxError: The while loop is missing an close key. \"}\"");
            }
            consequent.push(statement());
        }
        expect("}");
        advance();
        return {
            name: "whileLoop",
            condition,
            consequent
        }
    }

    function parseFor() {
        advance();
        expect("(");
        advance();
        let condition = [];
        if (currentToken.token == "var") {
            condition.push(statement());
            condition.push(logicExpression());
            condition.push(statement());
            advance();
        }
        expect("{");
        advance();
        let consequent = [];
        while (currentToken.token !== '}') {
            if (currentToken.name === 'EOF') {
                Print({ w: "SeholSyntaxError: The for loop is missing a closing brace '}'.", str: true });
                throw new Error("SeholSyntaxError: The for loop is missing a closing brace '}'.");
            }
            const result = statement();

            if (result) {
                consequent.push(result);
            } else {
                throw new Error(`Unexpected token in for loop body: ${JSON.stringify(currentToken)}`);
            }
        }
        expect("}");
        advance();
        return {
            name: "forLoop",
            condition,
            consequent
        }
    }

    function parseVarShortOp() {
        const id = currentToken.token;
        advance();
        const op = currentToken.token;
        advance();
        if (currentToken.token != ";") {
            Print({ w: `SeholGrammarError: After increment "${id}", we expect an ";", but it was "${currentToken.token}"`, str: true });
            throw new Error(`SeholGrammarError: After increment "${id}", we expect an ";", but it was "${currentToken.token}"`);
        }
        advance();
        return {
            name: "variableShortOp",
            id,
            op
        };
    }

    function parseReturnVarWithIndex() {
        const id = currentToken.token;
        advance();
        expect("[");
        advance();
        const indexArray = expression();
        expect("]");
        advance();
        return {
            name: "returnVarValue",
            id,
            index: indexArray
        }
    }

    function parseReturnVar() {
        const id = currentToken.token;
        advance();
        return {
            name: "returnVarValue",
            id
        }
    }

    function findClosingBracketIndex(startIndex) {
        let depth = 0;
        for (let i = startIndex; i < tree.length; i++) {
            if (tree[i].token === "[") {
                depth++;
            } else if (tree[i].token === "]") {
                depth--;
                if (depth === 0) {
                    return i;
                }
            }
        }
        throw new Error("Unmatched '[' found in statement.");
    }


    function statement() {
        if (currentToken.token == 'if') {
            return parseIf();
        } else if (currentToken.token == 'var') {
            return parseVar();
        } else if (currentToken.token == 'while') {
            return parseWhile();
        } else if (currentToken.token == 'for') {
            return parseFor();
        } else if (currentToken.name == 'Identifier') {
            // Operadores de incremento/decremento
            if (tree[index + 1].name === "incOp" || tree[index + 1].name === "descOp") {
                return parseVarShortOp();
            }

            // Atribuição de variável simples
            if (tree[index + 1].token !== "[" && tree[index + 1].token !== "(") {
                if (tree[index + 2]?.token === ':' || tree[index + 1]?.token === '=' || tree[index + 3]?.token === '=') {
                    return parseChangeVar();
                } else {
                    return parseReturnVar();
                }
            }

            // Acesso por índice em arrays
            if (tree[index + 1].token === "[") {
                // Verifica se é uma atribuição
                let closingBracketIndex = findClosingBracketIndex(index + 1);
                if (tree[closingBracketIndex + 1]?.token === '=') {
                    return parseChangeVarByIndex();
                } else {
                    return parseReturnVarWithIndex();
                }
            }

            // Função sendo chamada
            if (tree[index + 1].token === "(") {
                return parseFn();
            }
        }
        return expression();
    }

    let result = [];
    let run = true;
    while (run) {
        const call = statement();
        if (call) {
            result.push(call);
        } else {
            run = false;
        }
    }
    return result;
}
function resolveOperation(node) {
    // Caso base: é um número
    if (node.name == 'Number' || (node.token && node.token.name == 'Number')) {
        return parseInt(node?.token.token ? node.token.token : node.token, 10);
    }

    // Expressões matemáticas
    if (node?.name === 'expression') {
        let left = resolveOperation(node.left);  // Resolve o lado esquerdo
        let right = resolveOperation(node.right);  // Resolve o lado direito
        const areNumbers = typeThis(left) == 'Number' && typeThis(right) == 'Number';
        if (areNumbers) {
            left = parseInt(left, 10);
            right = parseInt(right, 10);
        }
        // Operadores suportados
        switch (node.op) {
            case 'plusOp':
                return left + right;
            case 'minusOp':
                return left - right;
            case 'timesOp':
                return left * right;
            case 'divOp':
                return left / right;
            default:
                throw new Error(`Unknown operator: ${node.op}`);
        }
    }

    // Expressões lógicas
    if (node?.name === 'logicExpression') {
        const left = resolveOperation(node.left);
        const right = node.right ? resolveOperation(node.right) : null;

        switch (node.op) {
            case 'eqOp':
                return left === right;
            case 'neqOp':
                return left !== right;
            case 'mqOp':
                return left > right;
            case 'mnqOp':
                return left < right;
            case 'andOp':
                return left && right;
            case 'orOp':
                return left || right;
            case 'notOp':
                return !left;
            default:
                throw new Error(`Unknown logical operator: ${node.op}`);
        }
    }

    // Identificadores (variáveis e arrays)
    if (node?.token?.name === 'Identifier') {
        const variable = varsTable[node.token.token]; // Busca a variável na tabela de variáveis

        if (variable) {
            const indexNode = node.index; // Índice do array, se houver
            if (indexNode) {
                // Resolve o índice (que pode ser uma expressão)
                const index = resolveOperation(indexNode);

                // Verifica se o índice está dentro do intervalo válido
                if (Array.isArray(variable.v) && index >= 0 && index < variable.v.length) {
                    return variable.v[index];
                } else {
                    throw new Error(`Index out of bounds for ${node.token.token}`);
                }
            }

            // Retorna o valor da variável se não for um array
            return variable.v;
        } else {
            throw new Error(`Undefined variable: ${node.token.token}`);
        }
    }

    // Valor padrão
    console.warn("Unrecognized node structure:", node);
    return node.token;
}


async function Run(ast, scoope = 0) {
    async function Input() {
        return new Promise((resolve) => {
            const input = document.getElementById("input-text");
            input.focus();
            input.addEventListener('keydown', function handler(e) {
                if (e.key === 'Enter') {
                    resolve([input, handler]);
                }
            });
        });
    }

    function timing(delay) {
        return new Promise(function (resolve) {
            setTimeout(resolve, delay);
        });
    }

    for (const node of ast) {
        if (node.call == "printLog") {
            for (const word of node.args) {
                if (typeof word.w == "object") {
                    const test = varsTable[word.w[1]];
                    if (test) {
                        word.w[1] = test.v;
                    }
                }
                Print(word, scoope);
            }
            Print({ w: '\n', str: true });
        } else if (node.call == "timing") {
            await timing(node.args[0].w);
        } else if (node.call == "input") {
            Terminal.innerHTML += `<pre-line><p>${node.args[0].w}</p><input type="text" id="input-text" focus/></pre-line>`;
            const [input, handler] = await Input()
            input.removeEventListener('keydown', handler);
            input.remove();
            return input.value;
        } else if (node.name == "ifStatement") {
            const conditionResult = resolveOperation(node.condition);
            if (conditionResult) {
                await Run(node.consequent, scoope);
            } else if (node.elseCondition) {
                await Run(node.elseCondition, scoope);
            }
        } else if (node.name == "whileLoop") {
            let result = resolveOperation(node.condition);
            while (result) {
                await Run(node.consequent, scoope);
                result = resolveOperation(node.condition);
            }
        } else if (node.name == "varDeclaration") {
            if (node.value.name == 'expression') {
                varsTable[node.id] = { v: resolveOperation(node.value), type: typeThis(node.value), scoope };
            } else {
                if (node.value.name == "fnCall") {
                    varsTable[node.id] = { v: await Run([node.value], scoope), type: typeThis(node.value), scoope };
                } else {
                    console.log(node.value, varsTable);
                    varsTable[node.id] = { v: node.value, type: node.type, scoope };
                }
            }
        } else if (node.name == "changeVarValue") {
            if (varsTable[node.id].scoope > scoope) {
                Print({ w: `SeholRuntimeError: Undefined variable "${node.id}" in the current scope.`, str: true });
                throw new Error(`SeholRuntimeError: Undefined variable "${node.id}" in the current scope.`);
            }
            let result = node.value;
            let index = node.index;
            if (result.name === "expression" || result.name === "Number" || result.name === "Identifier") {
                result = resolveOperation(node.value);
            }
            const resultType = typeThis(result.token);
            const variableType = varsTable[node.id].type;
            if (resultType != variableType && !node.index && variableType != "ListArray") {
                Print({ w: `SeholRuntimeError: You can't assign a value type "${resultType}" to the variable "${node.id}" that is type "${variableType}".`, str: true });
                throw new Error(`SeholRuntimeError: You can't assign a value type "${resultType}" to the variable "${node.id}" that is type "${variableType}".`);
            }
            if (resultType == "ListArray") {
                result = parseListArray(result);
            }
            if (index) {
                if (resultType == "nativeFn") {
                    result = await Run([result]);
                }
                varsTable[node.id].v[resolveOperation(index)] = result;
            } else {
                varsTable[node.id] = { v: result.token, type: variableType, scoope };
            }
        } else if (node.name == "variableShortOp") {
            if (varsTable[node.id].scoope > scoope) {
                Print({ w: `SeholRuntimeError: Undefined variable "${node.id}" in the current scope.`, str: true });
                throw new Error(`SeholRuntimeError: Undefined variable "${node.id}" in the current scope.`);
            }
            switch (node.op) {
                case "++":
                    varsTable[node.id].v++;
                    break;
                case "--":
                    varsTable[node.id].v--;
                    break;
            }
        } else if (node.name == "forLoop") {
            const v = node.condition[0].value;
            varsTable[node.condition[0].id] = { v: resolveOperation({ name: typeThis(v), token: v }), type: typeThis(v), scoope: scoope + 1 };
            let result = resolveOperation(node.condition[1]);
            while (result) {
                await Run(node.consequent, scoope + 1);
                await Run([node.condition[2]], scoope + 1);
                result = resolveOperation(node.condition[1]);
            }
        } else if (node.name == "returnVarValue") {
            if (index) {
                return varsTable[node.id].v[node.index];
            }
            return varsTable[node.id].v;
        } else {
            throw new Error("SeholRuntimeError: AST corrupted, member not found:" + node.name + ".")
        }
    }
}

document.getElementById('run').addEventListener('click', async () => {
    let code = Code.value;
    let tokens = Lexer(code);
    let ast = Parser(tokens);
    console.log(ast);
    Terminal.innerText = "";
    await Run(ast);
});