#include <string>
#include "stack.h"
#include "truthValStore.h"

using namespace std;

/**
 * @brief Returns precedence map of symbol
 * 
 * @param symbol 
 * @return int 
 */
int precedenceMap(char symbol) {
	switch (symbol) {
		case '~' : return 4; break;
		case '*' : return 3; break;
		case '+' : return 2; break;
		case '>' : return 1; break;
		default: return 0; break;
	}
}

bool isOperator(char symbol) {
	switch (symbol)
	{
	case '~':
	case '*':
	case '+':
	case '>':
		return true;
		break;
	default:
		return false;
		break;
	}
}

/**
 * @brief Implementation of logic methods
 * 
 */
class Logic {
    public:
		 /**
		 * @brief Task 1. Converts infix into prefix using stack.h 
		 * 
		 * @param infix String containing infix expression of a logical sequent
		 * @return String containing the corresponding prefix expression of the given infix form
		 */
		static string infixToPrefix(string infix) {
			size_t infixLength = infix.length();

			Stack<char> operatorStack(infixLength);
			Stack<string> operandStack(infixLength);

			for(int index = 0; size_t(index) < infixLength; index++) {
				char currentChar = infix[index];
				//debug("infixToPrefix currChar", currentChar);
				debug("Head in for", operandStack.peek());

				if (currentChar == '(') {
					operatorStack.push(currentChar);
				} else if (currentChar == ')') {
					while(!operatorStack.isEmpty() && (operatorStack.peek() != '(')) {
						if (operatorStack.peek() == '~') {
							string operand2 = operandStack.pop();
							operandStack.push(operatorStack.pop() + operand2);
						} else {
							string operand1 = operandStack.pop();
							string operand2 = operandStack.pop();
							operandStack.push(operatorStack.pop() + operand2 + operand1);
						}
					}
					operatorStack.pop();
				} else if (!isOperator(currentChar)) {
					operandStack.push(string(1, currentChar));
				} else {
					while(!operatorStack.isEmpty() && (precedenceMap(currentChar) <= precedenceMap(operatorStack.peek()))){
						if (operatorStack.peek() == '~') {
							string operand2 = operandStack.pop();
							operandStack.push(operatorStack.pop() + operand2);
						} else {
							string operand1 = operandStack.pop();
							string operand2 = operandStack.pop();
							operandStack.push(operatorStack.pop() + operand2 + operand1);
						}
					}
					operatorStack.push(currentChar);
				}
			}

			while(!operatorStack.isEmpty()) {
				debug("Head in while", operandStack.peek());
				// For some reason brackets were coming?
				if ((operatorStack.peek() == '(') || (operatorStack.peek() == ')')) {
					operatorStack.pop();
					continue;
				}

				// Checks for ~
				if (operatorStack.peek() == '~') {
					string operand2 = operandStack.pop();
					operandStack.push(operatorStack.pop() + operand2);
				} else {
					string operand1 = operandStack.pop();
					string operand2 = operandStack.pop();
					operandStack.push(operatorStack.pop() + operand2 + operand1);
				}
			}

			
			return operandStack.peek();
		};

		
		#include "operators.h"
		/**
		 * @brief Task 2. Prefix to parse tree
		 * 
		 * @param prefix String containing prefix expression of logical sequent
		 * @return Operator* Pointer to parse tree
		 */
		static Operator* prefixToParseTree(string prefix) {
			long unsigned int index = 0;
			return calcprefixToParseTree(prefix, &index);
		}
		
		 /**
		 * @brief Task 2. Prefix to parse tree
		 * 
		 * @param prefix String returned by task 1 containing prefix expression of the logical sequent
		 * @param index Pointer to index of the prefix string expression  
		 * @return Head of respective parse tree in each recursive call
		 */
		static Operator* calcprefixToParseTree(string prefix, long unsigned int* index) {
			debug("prefix coming to calcprefixToParseTree", prefix);
			debug("index coming to calcprefixToParseTree", *index);
			debug("prefix length", prefix.length());
			
			if (*index >= prefix.length()) {
				return nullptr;
			}
			char currentCharacter = prefix[*index];
			Operator* head = new Operator(currentCharacter);

			*index+=1;

			switch (currentCharacter) {
				case '~':
					head->addRightChild(calcprefixToParseTree(prefix, index));
					break;
				case '>':
				case '+':
				case '*':
					head->addLeftChild(calcprefixToParseTree(prefix, index));
					head->addRightChild(calcprefixToParseTree(prefix, index));
					break;
				default:
					break;
			}
			return head;
		};

		/**
		 * @brief Task 3. Traverses binary tree in order and evaluates infix
		 * 
		 * @param op 
		 * @return string 
		 */
		static string parseTreeToInfix(Operator* op) {
			string output;
			calcparseTreeToInfix(op, &output);
			return output;
		}

		/**
		 * @brief Task 3. Traverses binary tree in order and evaluates infix
		 * 
		 * @param op Pointer to parse tree
		 * @param output Pointer to string output which is updated to give the infix expression string in each recursive call
		 */
		static void calcparseTreeToInfix(Operator* op, string* output) {
			if (op == nullptr)
				return;
			if (op->isAtom()) {
				*output += op->getSymbol();
				debug("output per step", *output);
				return;
			}
			*output += '(';
			if (!op->isUnaryOperator()) calcparseTreeToInfix(op->getLeftChild(), output);
			*output += op->getSymbol();
			calcparseTreeToInfix(op->getRightChild(), output);
			*output += ')';
		};

		/**
		 * @brief Task 4. Get the Parse Tree Height object. Performs recursive evaluation on each child in O(n).
		 * 
		 * @param op Pointer to parse tree
		 * @return Tree height upto that particular node
		 */
		static int getParseTreeHeight(Operator* op) {
			if (op == nullptr)
				return 0;
			if (op->isAtom()) {
				return 1;
			}
			int leftChildHeight = 0;
			int rightChildHeight = 0;

			if (!op->isUnaryOperator()) {
				leftChildHeight = getParseTreeHeight(op->getLeftChild());
			}

			rightChildHeight = getParseTreeHeight(op->getRightChild());

			return max(leftChildHeight, rightChildHeight) + 1;
		}

		/**
		 * @brief Task 5. Get the Parse Tree Val object using a dictionary of symbols and their values
		 * 
		 * @param op Pointer to parse tree
		 * @param valueMap Dictionary mappiing symbols to their truth values
		 * @return true 
		 * @return false 
		 */
		static bool getParseTreeVal(Operator* op, TruthValStore valueMap) {
			debug("Symbol", op->getSymbol());
			debug("Value", valueMap.getTruthVal(op->getSymbol()));
			if (op->isAtom()) {
				return valueMap.getTruthVal(op->getSymbol());
			}

			switch (op->getSymbol()) {
				case '~':
					return !getParseTreeVal(op->getRightChild(), valueMap);	
				case '*':
					return (getParseTreeVal(op->getLeftChild(), valueMap) && getParseTreeVal(op->getRightChild(), valueMap));
				case '+':
					return (getParseTreeVal(op->getLeftChild(), valueMap) || getParseTreeVal(op->getRightChild(), valueMap));
				case '>':
					return (!getParseTreeVal(op->getLeftChild(), valueMap) && getParseTreeVal(op->getRightChild(), valueMap));
				default:
					return false;
			}
		};

		/**
		 * @brief Displays parse tree
		 * 
		 * @param op Pointer to parse tree
		 */
		static void displayParseTree(Operator* op) {
			int rec[Logic::getParseTreeHeight(op)];
			printParseTree(op, 0, rec);
			
		}

		/**
		 * @brief Helper function for displayParseTree
		 * 
		 * @param op 
		 * @param depth 
		 * @param rec 
		 */
		static void printParseTree(Operator* op, int depth, int* rec) {
			if(op == nullptr)
				return;
			
			cout << "     ";

			for(int childIndex = 0; childIndex < depth; childIndex++) {
				if(childIndex == depth - 1) {
					if ((rec[depth - 1] == 1) && !(op->isUnaryOperator())) {
						cout << "├───";
					} else {
						cout << "└───";
					}
				}
				else {
					if (rec[childIndex] == 1) {
						cout << "│   ";
					} else {
						cout << "     ";
					}
				}
			}
				
			
			cout << "[" << YELLOW << op->getSymbol() << RESET << "]" << endl;

			rec[depth]=1;

			printParseTree(op->getRightChild(), depth + 1, rec);

			rec[depth]=0;

			printParseTree(op->getLeftChild(), depth + 1, rec);
		}
};
