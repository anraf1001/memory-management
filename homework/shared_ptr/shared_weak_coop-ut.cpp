#include <gtest/gtest.h>

#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

struct Node {
    cs::shared_ptr<Node> child;
    cs::weak_ptr<Node> parent;
};

TEST(SharedWeakCoopTest, shouldSolveCycleDependecies) {
    cs::shared_ptr<Node> root(new Node{});
    cs::shared_ptr<Node> child(new Node{});

    root->child = child;
    child->parent = root;
}
