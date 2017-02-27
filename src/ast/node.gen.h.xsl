<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output omit-xml-declaration="yes" />
<xsl:template match="text()" mode="enum" />
<xsl:template match="text()" mode="struct" />
<xsl:template match="text()" mode="alloc" />

<xsl:template match="/">
<xsl:text disable-output-escaping="yes">
#pragma once
#include &lt;stdbool.h&gt;
#include "types.h"
</xsl:text>

enum nodetype_t {
    <xsl:apply-templates mode="enum" />
};

struct node {
    nodetype_t nodetype;
    int lineno, colno;

    union {
<xsl:apply-templates mode="struct" />
<xsl:text disable-output-escaping="yes">
    };
};
</xsl:text>
<xsl:apply-templates mode="alloc" />
</xsl:template>

<!-- This fills the enum of node types! -->
<xsl:template match="/ast/nodes/node" mode="enum">
    <xsl:value-of select="@name" />_n,
</xsl:template>

<!-- These templates define behaviour for the step where the struct definition
is generated. -->
<xsl:template match="/ast/nodes/node" mode="struct">
    struct {
        <xsl:apply-templates mode="struct" />
    } <xsl:value-of select="@name" />_n;
</xsl:template>

<xsl:template match="/ast/nodes/node/child" mode="struct">
    node *<xsl:value-of select="@name" />;
</xsl:template>

<xsl:template match="/ast/nodes/node/attribute" mode="struct">
    <xsl:variable name="attributetype" select="@attributetype"/>
    <xsl:value-of select="//ast/attributetypes/type[@name=$attributetype]/@ctype" />
    <xsl:text> </xsl:text>
    <xsl:value-of select="@name" />;
</xsl:template>

<!-- These templates define behaviour for the step where the allocation function
declarations are generated. -->
<xsl:template match="/ast/nodes/node" mode="alloc">
    node *alloc_<xsl:value-of select="@name" />(
        <xsl:apply-templates mode="alloc" />
    );
</xsl:template>

<xsl:template match="/ast/nodes/node/child" mode="alloc">
    node *<xsl:value-of select="@name" />
    <xsl:if test="position() != last() - 1">, </xsl:if>
</xsl:template>

<xsl:template match="/ast/nodes/node/attribute" mode="alloc">
    <xsl:variable name="attributetype" select="@attributetype"/>
    <xsl:value-of select="//ast/attributetypes/type[@name=$attributetype]/@ctype" />
    <xsl:text> </xsl:text>
    <xsl:value-of select="@name" />
    <xsl:if test="position() != last() - 1">, </xsl:if>
</xsl:template>
</xsl:stylesheet>
