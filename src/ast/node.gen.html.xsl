<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" version="1.0" encoding="utf-8" indent="yes"/>

<xsl:template match="/">
<head>
    <style>
        body {
    margin: auto;
    max-width: 650px;
    line-height: 1.6;
    font-size: 18px;
    padding: 10px;
    text-align: justify;
}

img {
    width: 100%;
}

nav ul {
    list-style-type: none;
    padding-left: 0;
}

nav li {
	display: inline;
}
</style>
</head>
<body>
<h1>Civic C AST</h1>

<h2 title="One table to rule them all"><a name="toc">Table of tables</a></h2>

<xsl:apply-templates mode="toc" />

<h2><a name="nodesets">Node sets</a></h2>

<xsl:apply-templates mode="nodesets" />

<h2><a name="nodes">Nodes</a></h2>

<xsl:apply-templates mode="nodes" />

</body>
</xsl:template>

<xsl:template match="/ast" mode="toc">
    <ul>
        <li><a href="#nodesets">Node sets</a></li>
        <ul>
            <xsl:for-each select="nodeset">
                <li><a>
                    <xsl:attribute name="href">#<xsl:value-of select="@name" /></xsl:attribute>
                    <xsl:value-of select="@name" />
                </a></li>
            </xsl:for-each>
        </ul>
        <li><a href="#nodes">Nodes</a></li>
        <ul>
            <xsl:for-each select="node">
                <li><a>
                    <xsl:attribute name="href">#<xsl:value-of select="@name" /></xsl:attribute>
                    <xsl:value-of select="@name" />
                </a></li>
            </xsl:for-each>
        </ul>
    </ul>
</xsl:template>

<xsl:template match="/ast/node" mode="nodes">
    <h3><a><xsl:attribute name="name"><xsl:value-of select="@name" /></xsl:attribute><xsl:value-of select="@name" /></a></h3>
    <div><xsl:value-of select="@description" /></div>
    <h4>Constructor</h4>
        <code>node *alloc_<xsl:value-of select="@name" />(
            <xsl:apply-templates mode="alloc" />
        );</code>
    <h4>Members</h4>
    <table>
        <tr>
            <th>Child</th>
            <th>Type</th>
            <th>Comment</th>
        </tr>
        <tr>
            <td>09:00 - 11:00</td>
            <td>Closed</td>
            <td>Open</td>
        </tr>
        <tr>
            <th>Attribute</th>
            <th>Type</th>
            <th>Comment</th>
        </tr>
        <tr>
            <td>11:00 - 13:00</td>
            <td>Open</td>
            <td>Open</td>
        </tr>
    </table>

    <hr />
</xsl:template>

<xsl:template match="/ast/node" mode="alloc">

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

<xsl:template match="/ast/nodeset" mode="nodesets">
    <h3><a><xsl:attribute name="name"><xsl:value-of select="@name" /></xsl:attribute><xsl:value-of select="@name" /></a></h3>
    {<xsl:apply-templates mode="nodesets" />}
    <hr />
</xsl:template>

<xsl:template match="/ast/nodeset/node" mode="nodesets">
    <a>
        <xsl:attribute name="href">#<xsl:value-of select="@target" /></xsl:attribute>
        <xsl:value-of select="@target" />
    </a>
    <xsl:if test="position() != last() - 1">, </xsl:if>
</xsl:template>

</xsl:stylesheet>
