package ija.ija2017.homework1.items;

public class Item01 extends AbstractItem {

    public Item01(String name, int port) {
        super(name, port);
    }

    @Override
    public void execute() {
        super.state++;
    }
}
