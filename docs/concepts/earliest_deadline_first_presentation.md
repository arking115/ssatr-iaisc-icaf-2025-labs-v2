# Earliest Deadline First (EDF)

## Conceptul de bază

Earliest Deadline First este un algoritm de planificare **dinamic** pentru sisteme de timp real. La fiecare moment de timp, se execută **taskul cu cel mai apropiat deadline absolut**.

### Regula fundamentală

- La orice moment, se alege taskul cu **deadline-ul absolut cel mai mic**
- Prioritatea se **recalculează dinamic** pe măsură ce taskurile sunt create și finalizate
- Spre deosebire de RMS și DMS, prioritățile nu sunt **fixe în avans**

### De ce este diferit?

- **RMS/DMS** = algoritmi cu prioritate statică (se atribuie o dată)
- **EDF** = algoritm cu prioritate dinamică (se schimbă în timp real)

**Exemplu:** Dacă Task A are deadline la 50ms și Task B la 30ms, Task B se execută primul indiferent de alte caracteristici.

## Avantaje și caracteristici

### De ce este util EDF?

✓ **Algoritm optim** - utilizare maximă a CPU în cazuri în care RMS/DMS eșuează
✓ **Flexibil** - funcționează cu taskuri periodice și aperiodice
✓ **Dinamic** - se adaptează la runtime la schimbări în sistem
✓ **Utilizare superioară** - până la 100% utilizare pentru taskuri periodice
✓ **Simplicitate** - ușor de implementat într-un sistem real

### Proprietăți importante

- EDF este **optimal** pentru planificarea taskurilor periodice și aperiodice cu deadline-uri relative
- Dacă un sistem este planificabil, atunci EDF îl va planifica cu succes
- Funcționează atât offline (cu schedule cunoscut) cât și online (dinamic)
- Preempțiune: dacă apare un task cu deadline mai mic, acesta întrerupe taskul curent

## Testul de planificabilitate

Pentru taskuri periodice, testul de utilizare este **mult mai simplu** decât la RMS/DMS:

$$U = \sum_{i=1}^{n} \frac{C_i}{T_i} \leq 1$$

Unde:
- **Cᵢ** = timp de execuție al taskului i
- **Tᵢ** = perioada taskului i
- **n** = numărul de taskuri

### Interpretare

- Dacă **U ≤ 1** (100%) → sistemul este cu siguranță planificabil
- EDF poate utiliza CPU la 100%, spre deosebire de RMS/DMS (max ~69%)

## Comparație cu RMS/DMS

| Algoritm | Condiție planificabilitate | Utilizare maximă |
|----------|--------------------------|------------------|
| **RMS** | U ≤ n(2^(1/n) - 1) | ~69% |
| **DMS** | U ≤ n(2^(1/n) - 1) | ~69% |
| **EDF** | U ≤ 1 | 100% |

### Exemplu practic

3 taskuri cu U = (1/4) + (1/5) + (1/6) = 0.783:
- RMS/DMS: U < 0.780? Nu, sistemul poate eșua ❌
- EDF: U < 1? Da, sistemul este garantat planificabil ✓
