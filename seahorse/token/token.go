package Token

const (
	TOKEN_ID      = iota
	TOKEN_OP      = iota
	TOKEN_STRING  = iota
	TOKEN_NUMERIC = iota
	TOKEN_EOF     = iota

	// Delimiter
	TOKEN_OPENPAREN    = iota
	TOKEN_CLOSEPAREN   = iota
	TOKEN_OPENCURLY    = iota
	TOKEN_CLOSECURLY   = iota
	TOKEN_OPENBRACKET  = iota
	TOKEN_CLOSEBRACKET = iota
	TOKEN_PER          = iota
	TOKEN_COLON        = iota
	TOKEN_SEMI         = iota

	// Operation
	TOKEN_PLUS   = iota
	TOKEN_MINUS  = iota
	TOKEN_MUL    = iota
	TOKEN_DIV    = iota
	TOKEN_MOD    = iota
	TOKEN_ASSIGN = iota
	TOKEN_EQ     = iota
	TOKEN_NE     = iota
	TOKEN_GT     = iota
	TOKEN_LT     = iota
	TOKEN_GE     = iota
	TOKEN_LE     = iota
)

type Token struct {
	Kind  int
	Value string
	Index int
}
