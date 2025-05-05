# Codegen Redesign Plan

## NOTES:

Below is a list of conceptualized ideas on how to make Aly's codegen better and work in a non-platform specific way,
and this is what I will be working on next, I believe. Honestly, we just need a code generator that doesn't suck :).

To do this, I think I must start with some dynamic register structure that I can search for register data within just by
a simple number. This could be a hash table, an array, an environment, or whatever. All I care about is that we can mark/unmark
registers as in use, get a register currently not in use, as well as get a register name string. If no usable registers are found
when one is queried for, just error for now. Eventually, we can figure out what to do in this special case.

We also need to create a label generator; something that can generate assembly labels that do not overlap. To do this, we can use
a the traditional/convention, which is to name them `.L` follow by an integer. The generation will be quiet simple; have a global
array that is written to with a global integer value, incremented each time. Return the pointer to this array, and everything will
work as long as multiple labels are not generated concurrently and used, if that makes sense.

## Simple CodeGen Steps (post-order traversal):
1. For each node, generate children in order.
2. ^Allocate register for result.
	Re-use a child's result register, if possible (destructive).
3. Emit instruction using registers (write to file).
4. Free registers no longer necessary.

Each node is independent, expect for registers carrying values along edge (connection between nodes).

## Futher Notes

When an assembly instruction is destructive (for example, ADD overwrites one of it's register operands, adding into it),
then we are able to re-use one of the children's result register, as we will be destroying it anyway by emitting the ADD instruction
with that register.

When encountering a symbol, we need some way to convert it to where it is stored. This will either be a literal symbol that is RIP
addressed (`symbol(%rip)`), or a base pointer offset (`-N(%rbp)`).
We should create a function that can convert a symbol node into one of these string things that is able to be emitted. When could call
this `codegen_symbol(Node* symbol)`. It seems we will need some sort of codegen context, otherwise where do we look for the symbol? We may
be able to get away with storing state internally to the function using globals or something.

I think we actually will need some extra data that will end up being passed around as codegen context. We need the ability to determine the
base pointer offset of a variable based on symbol alone. To do this, it will require some sort of environment where variable symbols are
mapped to base pointer offsets. There is no other way, right?
If there is, we'll find it when we fuck up and do it the wrong way :^).

For result handling to work properly, we must store the result register in the AST node. This allows hte parent to access the result registers
of it's children, which is especially necessary for destructive instructions like ADD.

## New CodeGen Example

```c
	typedef struct CodegenContext {
		struct CodegenContext* parent;
		// LOCALS
		// `-- SYMBOL (NAME) -> INTEGER (BASE POINTER OFFSET)
		Environment* locals;
	} CodegenContext;

	#define CG_SYMBOL_BUFFER_SIZE 1024
	char codegen_symbol_buffer[CG_SYMBOL_BUFFER_SIZE];
	char* codegen_symbol(CodegenContext* context, Node* symbol, char global) {
		if (global) {
			snprintf(codegen_symbol_buffer, CG_SYMBOL_BUFFER_SIZE, "%S(%%rip)", symbol->value.symbol);
		} else {
			Node* symbol_offset = node_allocate();
			// Would it be possible to store base pointer offset of each stack frame, allow for locals accessed above current scope?
			// No, we'd have to calculate RSP - RBP at runtime, and then store that result in a register or something....
			environment_get(*context->locals, symbol, symbol_offset);
			snprintf(codegen_symbol_buffer, CG_SYMBOL_BUFFER_SIZE, "%S(%%rbp)", symbol->value.symbol);
		}
		return codegen_symbol_buffer;
	}
```

```c
	typedef int RegisterDescriptor;

	typedef struct Register {
		struct Register* next;
		char* name;
		char in_use;
	} Register;

	void register_add(Register* base, char* name) {
		if (!base || !name) {
			printf("ERROR::register_add(): NULL args\n");
			return;
		}
		while (r->next) {
			r = r->next;
		}
		r->next = malloc(sizeof(Register));
		r = r->next;
		if (!r) {
			printf("Could not allocate new register: \"%s\"\n", name);
			exit(1);
		}
	}

	RegisterDescriptor register_allocate(Register* r)
		RegisterDescriptor out = 0;
		while (r->next) {
			if (!r->in_use) { return out; }
			r = r->next;
			out++;
		}
		printf("Ran out of registers to allocate!\n");
		exit(1);
		return -1;
	}

	void register_free(Register* r, RegisterDescriptor register_descriptor) {
		Register* previous = NULL;
		for (RegisterDescriptor i = 0; i < register_descriptor; ++i) {
			previous = r;
			r = r->next;
		}
		r->in_use = 0;
	}
```

## Compiler Design

In our case, we are targeting assembly language, but eventually we would like to target relocatable machine languages, if we ever get
that far :^p.