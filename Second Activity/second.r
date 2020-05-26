#Desabilite a linha abaixo caso o pacote ainda n�o foi instalado no sistema
install.packages("FrF2", repos = "http://cran.rstudio.com/") # Instala o pacote

library(FrF2) # Carrega o pacote para uso


#exemplo do slide (retirado do livro do Jain)
plan.person = FrF2(nruns = 4,
                   nfactors =  2,
                   replications = 1,
                   repeat.only = FALSE,
                   factor.names = list(
                     Technique = c("interchange", "unrolling"),
                     Matrix_Size = c("100", "1000")),
                   randomize = FALSE)

summary(plan.person)

# Variavel de resposta: Cache-Loads
#Cache_Loads = c(27965259.85,27079575553,25958001.5,24985846087)
# Variavel de resposta: Cache_Misses
#Cache_Misses = c(85282,70965097,102139,1313128643)
# Variavel de resposta: Branch-Loads
#Branch_Loads = c(1615342.75, 1048737075, 1116725.85, 551085250.2)
# Variavel de resposta: Branch-Misses
Branch_Misses = c(19899.45,1419769.1,11336.15,1671143.45)

plan.atualizado = add.response(design = plan.person, response = Branch_Misses)

summary(plan.atualizado)

MEPlot(plan.atualizado)

IAPlot(plan.atualizado)


# Modelo linear = Branch_Misses depende (~) de Rede e Acesso
plan.formula = lm(plan.atualizado$Branch_Misses~(plan.atualizado$Tecnica*plan.atualizado$Tamanho))

# A tabela fornecida na linha abaixo diz na coluna "Estimate" os valores de q0 (media), qA (Redes1), qB (Acesso1) e qAB (Redes e Acesso)
summary(plan.formula)

# Calculo das Somas dos Quadrados
plan.anova = anova(plan.formula)

# Valores das medias dos quadrados podem ser observados na tabela gerada pela linha abaixo
# na coluna "Mean Sq". 
summary(plan.anova$`Mean Sq`)

# Calculo da Soma dos Quadrados Total
SST = plan.anova$"Mean Sq"[1] + plan.anova$"Mean Sq"[2] + plan.anova$"Mean Sq"[3]

#Calculo da influencia de cada fator
InfluenciaA = plan.anova$"Mean Sq"[1] / SST
InfluenciaB = plan.anova$"Mean Sq"[2] / SST
InfluenciaAB = plan.anova$"Mean Sq"[3] / SST

cat("Influencia devido ao fator Tecnica:", InfluenciaA)
cat("Influencia devido ao fator Tamanho da Matriz:", InfluenciaB)
cat("Influencia devido a interação dos fatores:", InfluenciaAB)

