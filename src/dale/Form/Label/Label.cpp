#include "../../Generator/Generator.h"
#include "../../Node/Node.h"
#include "../../ParseResult/ParseResult.h"
#include "../../Element/Function/Function.h"
#include "llvm/Function.h"

namespace dale
{
namespace Form
{
namespace Label
{
bool execute(Generator *gen,
             Element::Function *fn,
             llvm::BasicBlock *block,
             Node *node,
             bool get_address,
             bool prefixed_with_core,
             ParseResult *pr)
{
    Context *ctx = gen->ctx;

    assert(node->list && "parseLabel must receive a list!");

    if (!ctx->er->assertArgNums("label", node, 1, 1)) {
        return false;
    }

    symlist *lst = node->list;
    Node *lname = (*lst)[1];

    if (!ctx->er->assertArgIsAtom("label", lname, "1")) {
        return false;
    }
    if (!ctx->er->assertAtomIsSymbol("label", lname, "1")) {
        return false;
    }
    Token *t = lname->token;

    /* Create a block with this name. */

    llvm::BasicBlock *new_block =
        llvm::BasicBlock::Create(llvm::getGlobalContext(),
                                 t->str_value.c_str(),
                                 block->getParent());

    /* If the existing block does not terminate, create a branch to
     * this label. */

    if (block->size() == 0
            || !(block->back().isTerminator())) {
        llvm::IRBuilder<> builder(block);
        builder.CreateBr(new_block);
    }

    if (fn->getLabel(t->str_value.c_str())) {
        Error *e = new Error(
            ErrorInst::Generator::RedeclarationOfLabel,
            node,
            t->str_value.c_str()
        );
        ctx->er->addError(e);
        return false;
    }

    int index = ctx->ns()->lv_index++;
    Element::Label *my_label = new Element::Label();
    my_label->block = new_block;
    my_label->ns = ctx->ns();
    my_label->index = index;
    fn->addLabel(t->str_value.c_str(), my_label);
    
    pr->set(new_block, ctx->tr->getBasicType(Type::Int),
            llvm::ConstantInt::get(ctx->nt->getNativeIntType(), 0));

    return true;
}
}
}
}