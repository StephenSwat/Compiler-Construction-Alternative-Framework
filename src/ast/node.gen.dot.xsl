<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" version="1.0" encoding="utf-8" indent="yes"/>

<xsl:template match="/">
    digraph my_ast {
        <xsl:apply-templates />
    }
</xsl:template>

<xsl:template match="/ast/node">
    <xsl:variable name="name"><xsl:value-of select="@name"/></xsl:variable>

    "<xsl:value-of select="$name" />" [label="&lt;core&gt; <xsl:value-of select="@name" />
    <xsl:for-each select="attribute">|<xsl:value-of select="@name" />
    </xsl:for-each>" shape="record"];

    <xsl:for-each select="child">
        <xsl:variable name="label"><xsl:value-of select="@name"/></xsl:variable>
        <xsl:value-of select="$name" /> -> <xsl:value-of select="@target"/>
        [label="<xsl:value-of select="$label"/>" <xsl:if test="@optional='true'">style="dashed"</xsl:if>];
    </xsl:for-each>
</xsl:template>

<xsl:template match="/ast/nodeset">
    <xsl:variable name="name"><xsl:value-of select="@name"/></xsl:variable>
    <xsl:for-each select="node">
        <xsl:value-of select="$name" /> -> <xsl:value-of select="@target"/> [color=blue];
    </xsl:for-each>
</xsl:template>

</xsl:stylesheet>
