<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output omit-xml-declaration="yes" />
<xsl:template match="text()" mode="alloc"/>
<xsl:template match="text()" mode="alloc_inner"/>
<xsl:template match="text()" mode="nodeset"/>
<xsl:template match="text()" mode="nodetype_string"/>
<xsl:template match="text()" mode="traverse_children"/>

<xsl:template match="/">
<xsl:text disable-output-escaping="yes">
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include "main.h"
#include "traverse.h"
#include "node.gen.h"
</xsl:text>

<xsl:apply-templates mode="nodeset" />

<xsl:text disable-output-escaping="yes">
static node *alloc_node(nodetype_t type) {
    node *result = (node *) malloc(sizeof(node));
    result->nodetype = type;
    result->lineno = line;
    result->colno = col;
    return result;
}
</xsl:text>

<xsl:apply-templates mode="alloc" />

char *nodetype_string(nodetype_t t) {
    switch (t) {
        <xsl:apply-templates mode="nodetype_string" />
        default: return "unknown";
    }
}

node *traverse_children(node * this, info * info) {
    switch (<xsl:text disable-output-escaping="yes">this-&gt;</xsl:text>nodetype) {
        <xsl:apply-templates mode="traverse_children" />
        default:
            break;
    }

    return this;
}
</xsl:template>

<!-- Some functions to test if a node is part of a nodeset -->
<xsl:template match="/ast/nodeset" mode="nodeset">
    static bool node_is_<xsl:value-of select="@name" />(nodetype_t in) {
        switch (in) {
            <xsl:for-each select="node">
            case <xsl:value-of select="@target" />_n:
            </xsl:for-each>
                return true;
            default:
                return false;
        }
    }
</xsl:template>

<!-- These templates define behaviour for the step where the allocation function
definitions are generated. -->
<xsl:template match="/ast/node" mode="alloc">
    node *alloc_<xsl:value-of select="@name" />(
        <xsl:apply-templates mode="alloc" />
    ) {
        node *this = alloc_node(<xsl:value-of select="@name" />_n);
        <xsl:apply-templates mode="alloc_inner" />
        return this;
    }
</xsl:template>

<xsl:template match="/ast/node/child" mode="alloc">
    node *<xsl:value-of select="@name" />
    <xsl:if test="position() != last() - 1">, </xsl:if>
</xsl:template>

<xsl:template match="/ast/node/attribute" mode="alloc">
    <xsl:value-of select="@type" />
    <xsl:text> </xsl:text>
    <xsl:value-of select="@name" />
    <xsl:if test="position() != last() - 1">, </xsl:if>
</xsl:template>

<xsl:template match="/ast/node/child|/ast/node/attribute" mode="alloc_inner">
    <xsl:text disable-output-escaping="yes">this-&gt;</xsl:text><xsl:value-of select="../@name" />_n.
    <xsl:value-of select="@name" />=<xsl:value-of select="@name" />;
</xsl:template>

<!-- This one does the nodetype_string function. -->
<xsl:template match="/ast/node" mode="nodetype_string">
    case <xsl:value-of select="@name" />_n: return "<xsl:value-of select="@name" />";
</xsl:template>

<!-- and... the traverse_children function. -->
<xsl:template match="/ast/node" mode="traverse_children">
    case <xsl:value-of select="@name" />_n:
        <xsl:apply-templates mode="traverse_children" />
        break;
</xsl:template>

<xsl:template match="/ast/node/child" mode="traverse_children">
    <xsl:text disable-output-escaping="yes">this-&gt;</xsl:text><xsl:value-of select="../@name" />_n.
    <xsl:value-of select="@name" />=traverse_init(
    <xsl:text disable-output-escaping="yes">this-&gt;</xsl:text>
    <xsl:value-of select="../@name" />_n.<xsl:value-of select="@name" />, info);
</xsl:template>
</xsl:stylesheet>
